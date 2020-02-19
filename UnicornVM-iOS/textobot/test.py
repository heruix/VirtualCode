# -*- coding: utf-8 -*-
'''
This file is used to test UnicornVM iOS arm64 library's API.

You can run this file using Textobot.

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

import sys
import subprocess

binRoot = '/tmp'
testBin = binRoot + '/unicornvm_apitest'

def launch(cmdarr):
    print(cmdarr)
    return subprocess.Popen(cmdarr,stdout=subprocess.PIPE)

def main():
    proc = launch([testBin])
    proc.wait()
    print(proc.stdout.read())
    
main()
