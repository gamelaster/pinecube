import 'package:dart_clipboard/dart_clipboard.dart';

void main() {
  String clipboard = Clipboard.getContents();
  //String clipboard =
  //    "  *(_DWORD *)&h264Context->gapC[468] = 18;\n*(_DWORD *)&h264Context->gapC[428] = 0x200000;\n*(_DWORD *)&h264Context->gapC[432] = 30000;\n*(_DWORD *)&h264Context->gapC[420] = 25;\n*(_DWORD *)&h264Context->gapC[448] = 45;";
  clipboard = clipboard
      .replaceAllMapped(RegExp(r'\-\>gap([A-F0-9]+)\[([0-9]+)\]'), (match) {
    String address =
        (int.parse(match.group(1), radix: 16) + int.parse(match.group(2)))
            .toRadixString(16)
            .toUpperCase();
    return "->field_$address";
  });
  print("Converted :-)");
  print(clipboard);
  Clipboard.setContents(clipboard);
}
