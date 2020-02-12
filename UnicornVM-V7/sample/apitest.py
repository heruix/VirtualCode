# -*- coding: utf-8 -*-
'''
This file is used to test UnicornVM android armeabi-v7a library's API.

You can run this file using Fridobot.

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

import subprocess

binRoot = '/data/local/tmp/LidaDbgServer-arm'
testBin = binRoot + '/unicornvm_apitest'

def launch(cmdarr, envdict=None):
    if not envdict:
        envdict = {
            'LD_LIBRARY_PATH' : binRoot, 
        }
    print(envdict)
    print(cmdarr)
    return subprocess.Popen(cmdarr, env=envdict)
    
if __name__ == '__main__':
    launch([testBin]).wait()
