#from idaapi import PluginForm
from PyQt5.Qt import QApplication
import sip

#print("HII")

#from PyQt5.Qt import QApplication

def copy_to_clip(data):
    QApplication.clipboard().setText(data)

class Printer(idaapi.text_sink_t):
    def __init__(self):
        try:
            idaapi.text_sink_t.__init__(self)
        except AttributeError:
            pass  # Older IDA versions keep the text_sink_t abstract
        self.lines = []

    def _print(self, thing):
        self.lines.append(thing)
        return 0


def local_types():
    printer = Printer()
    idaapi.print_decls(printer, idaapi.cvar.idati, [80], 0)
#                       idaapi.PDF_INCL_DEPS | idaapi.PDF_DEF_FWD)
    return printer.lines

def add_comments(text):
    context = idaapi.get_struc_id("h264_context")
    sofs = idc.get_first_member(context)
    while sofs<=idc.get_last_member(context):
        if idc.get_member_id(context, sofs) != -1:
            reg = r"( {0}(\[[0-9]+\])?;)".format(idc.get_member_name(context, sofs))
            comment = r'\1 // field_{0:X} - {1}'.format(sofs, idc.get_member_cmt(context, sofs, 1))
            text = re.sub(reg, comment, text) 
            #print(hex(sofs) , " ", idc.get_member_name(context, sofs), " ", idc.get_member_cmt(context, sofs, 1))
        sofs = idc.get_next_offset(context, sofs)
    return text

def export_h264():
    text = '\n'.join(local_types()).replace('_BYTE', 'char').replace('\n/* 80 */\n', '').replace('};\n\n', '} h264_context;').replace('struct h264_context', 'typedef struct ').replace('_DWORD', 'int')
    text = add_comments(text)
    QApplication.clipboard().setText(text)
    print("Copied...")
export_h264()