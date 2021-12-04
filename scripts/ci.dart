import 'dart:io';
import 'package:path/path.dart' as path;

class FunctionInfo {
  String name;
  Map<String, String> arguments;

  FunctionInfo({this.name, this.arguments});
}

Future<List<FunctionInfo>> parseCode() async {
  final files = (await Directory(path.absolute(
              path.dirname(Platform.script.toFilePath()),
              "../software/recedar/"))
          .list()
          .toList())
      .where((element) => element.path.endsWith('.c'));

  List<FunctionInfo> functions = [];
  for (final file in files) {
    final codeLines = await File(file.path).readAsLines();
    for (int i = 0; i < codeLines.length; i++) {
      final line = codeLines[i];
      if (line.contains("// -RC ")) {
        final arguments = Map<String, String>.fromIterable(
          line.split("// -RC ")[1].split(',').map((e) => e.split('=')),
          key: (element) => element[0],
          value: (element) => element[1],
        );
        RegExp functionNameExp = RegExp(r" ([a-zA-Z0-9_]+)\((.*?)\)");
        final functionName = functionNameExp
            .firstMatch(codeLines[++i].replaceAll('*', ''))
            .group(1);
        functions.add(FunctionInfo(name: functionName, arguments: arguments));
      }
    }
  }
  return functions;
}

void main() async {
  final functions = await parseCode();
  List<String> table = ["| Function | Status |", "|----------|:-----:|"];
  int totalFunctions = 0;
  int implementedFunctions = 0;
  for (final function in functions) {
    String state = '';
    switch (function.arguments['status']) {
      case 'hooked':
        state = '🔀 Hooked (Work in Progress)';
        break;
      case 'implementing':
        String percent = function.arguments['percent'] ?? '?';
        state = "🔁 Implementing ($percent% done)";
        break;
      case 'implemented':
        implementedFunctions++;
        state = '✅ Implemented';
        break;
    }
    totalFunctions++;
    table.add("| ${function.name} | $state |");
  }

  String percentageDone =
      (implementedFunctions.toDouble() / totalFunctions.toDouble())
          .toStringAsPrecision(2);
  String info =
      "There is at the moment ${totalFunctions} functions found, where ${implementedFunctions} functions are implemented (${percentageDone}%)";
  String area = info + '\n\n' + table.join('\n');

  final readmeFilePath = path.absolute(
      path.dirname(Platform.script.toFilePath()),
      "../software/recedar/README.md");
  String readme = await File(readmeFilePath).readAsString();
  final startTag = '<!-- BEGIN STATE AREA -->';
  final endTag = '<!-- END STATE AREA -->';
  readme = readme.replaceRange(readme.indexOf(startTag) + startTag.length,
      readme.indexOf(endTag), '\n' + area + '\n');
  await File(readmeFilePath).writeAsString(readme);
}
