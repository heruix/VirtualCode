# -*- coding: utf-8 -*-
'''
This file is used to test UnicornVM android arm64-v8a library.

The injected file hijackmain.so will hijack elf's main function 
which will run under UnicornVM' virtual cpu context.

See jni/hijackmain.cpp for more information.

If you have any advises, We are happy to hear from you.


Follow us:
----------------------------------------------------------------------
        Email                           971159199@qq.com
        公众号                           刘柏江
        头条                             刘柏江
        百家号                           刘柏江VM
        微博                             刘柏江VM
        码云                             https://gitee.com/geekneo/
----------------------------------------------------------------------
'''

import os
import sys
import subprocess

tmpRoot = '/data/local/tmp'
binRoot = tmpRoot + '/LidaDbgServer-arm64'
elfPatcher = binRoot + '/elfpatcher'

def shell(cmds):
    print(cmds)
    os.system(cmds)

def execmd(cmdarr):
    print(cmdarr)
    proc = subprocess.Popen(cmdarr, stdout=subprocess.PIPE)
    proc.wait()
    return proc.stdout.read()

def vm_launch(cmdarr, envdict=None):
    exepath = cmdarr[0]
    if not os.path.exists(exepath):
        print('There is no %s.' % (exepath))
        return None
    
    vmpath = tmpRoot + '/uvm_' + os.path.basename(exepath)
    if not os.path.exists(vmpath):
        shell('cp -p %s %s; %s --add-needed libhijackmain.so %s' % (exepath, vmpath, elfPatcher, vmpath))
        dtlist = execmd([elfPatcher, '--print-needed', vmpath])
        if dtlist.find('hijackmain') < 0:
            shell('rm ' + vmpath)
            print('%sFailed to make vm version executable file %s.' % (dtlist, vmpath))
            return None
        
    if not envdict:
        envdict = {
            'LD_LIBRARY_PATH' : binRoot,
        }
    cmdarr[0] = vmpath
    print('ENV: ' + str(envdict))
    print('CMD: ' + str(cmdarr))
    return subprocess.Popen(cmdarr, env=envdict)

def main(args):
    if len(args) == 1:
        # set default test program
        args = [
            args[0], 
            elfPatcher, 
        ]
    exepath = args[1]
    if not os.path.exists(exepath):
        print('%s do not exist.' % (exepath))
        return
    vm_launch(args[1:len(args)]).wait()
    
if __name__ == '__main__':
    main(sys.argv)
