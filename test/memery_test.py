#!/usr/bin/env python
# coding=utf-8
'''
@Author: ArlenCai
@Date: 2019-11-04 22:00:53
@LastEditTime: 2019-11-05 21:17:34
'''
while(True):
    import cppjiebadat_py as jieba
    import cppjiebadat_py.analyse as analy
    import cppjiebadat_py.posseg as pseg

    #jieba.load_userdict("./userdict.txt")
    jieba.initialize()

    jieba.lcut("我来到北京清华大学", HMM=True)

    pseg.lcut("我来到北京清华大学")

    pseg.lcut("李小福是创新办主任也是云计算方面的专家")

    s = "此外，公司拟对全资子公司吉林欧亚置业有限公司增资4.3亿元，增资后，吉林欧亚置业注册资本由7000万元增加到5亿元。吉林欧亚置业主要经营范围为房地产开发及百货零售等业务。目前在建吉林欧亚城市商业综合体项目。2013年，实现营业收入0万元，实现净利润-139.13万元。"
    analy.extract_tags(s, topK=10, withWeight=True)
    analy.textrank(s, topK=10, withWeight=True)
