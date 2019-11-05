#!/usr/bin/env python
# coding=utf-8
'''
@Author: ArlenCai
@Date: 2019-10-31 14:30:28
@LastEditTime: 2019-11-05 17:32:04
'''
import cppjiebadat_py

def cut(sentence, HMM=False):
    it = cppjiebadat_py.tag(sentence)
    return iter(it)

def lcut(sentence, HMM=False):
    return cppjiebadat_py.tag(sentence)
