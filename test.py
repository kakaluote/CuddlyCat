# -*-coding:utf-8-*-
import glob
import os
import shutil

import util


# XML_DIR = '/home/kakaluote/2T/dataset/goods/no_label'
# TARGET_DIR = '/home/kakaluote/2T/dataset/goods/no_label1'
#
# xml_files = glob.glob(os.path.join(XML_DIR,"*.xml"))
#
# for xml_file in xml_files:
#     print(xml_file)
#     filename = util.get_filename(xml_file)
#     img_file = os.path.join(XML_DIR,filename+'.jpg')
#
#     shutil.copy(xml_file,os.path.join(TARGET_DIR,filename+'.xml'))
#     shutil.copy(img_file,os.path.join(TARGET_DIR,filename+'.jpg'))


# 示例 1:
#
# 输入:
# s = "aa"
# p = "a"
# 输出: false
# 解释: "a" 无法匹配 "aa" 整个字符串。
# 示例 2:
#
# 输入:
# s = "aa"
# p = "a*"
# 输出: true
# 解释: 因为 '*' 代表可以匹配零个或多个前面的那一个元素, 在这里前面的元素就是 'a'。因此，字符串 "aa" 可被视为 'a' 重复了一次。
# 示例 3:
#
# 输入:
# s = "ab"
# p = ".*"
# 输出: true
# 解释: ".*" 表示可匹配零个或多个（'*'）任意字符（'.'）。
# 示例 4:
#
# 输入:
# s = "aab"
# p = "c*a*b"
# 输出: true
# 解释: 因为 '*' 表示零个或多个，这里 'c' 为 0 个, 'a' 被重复一次。因此可以匹配字符串 "aab"。
# 示例 5:
#
# 输入:
# s = "mississippi"
# p = "mis*is*p*."
# 输出: false


class Solution:
    def isMatch(self, s: str, p: str) -> bool:
        j = 0
        i = 0
        while i < len(p):
            if p[i] == '*':
                return False
            if j >= len(s):
                return False

            # 有下一个字符
            if i + 1 < len(p):
                # 下个字符不是*
                if p[i + 1] != '*':
                    if p[i] != '.' and p[i] != s[j]:
                        return False
                    j += 1

                # 下个字符是*
                else:
                    k = j
                    while k < len(s) and s[k] == s[j]:
                        k += 1
                    if p[i] != '.' and p[i] == s[j]:
                        j = k
                    if p[i] == '.':
                        j = len(s)

                i += 2

            # 没有下一个字符
            else:
                if p[i] != '.' and s[j] != p[i]:
                    return False

                j += 1
                i += 1

        # 没有匹配完
        if j < len(s):
            return False

        return True

    def isMatch2(self, s: str, p: str) -> bool:
        j = 0
        i = 0
        while i < len(p):
            if p[i] == '*':
                return False
            if j >= len(s):
                break

            if i + 1 < len(p):
                if p[i + 1] != '*':
                    if p[i] != '.' and p[i] != s[j]:
                        return False
                    j += 1
                    i += 1
                else:
                    k = j
                    while k < len(s) and s[k] == s[j]:
                        k += 1
                    if p[i] != '.' and p[i] == s[j]:
                        j = k
                    if p[i] == '.':
                        j = len(s)

                    i += 2
            else:
                if p[i] != '.' and s[j] != p[i]:
                    return False

                j += 1
                i += 1

        if j < len(s):
            return False

        if i < len(p) and p[i] != s[-1]:
            return False

        return True


# print(Solution().isMatch2("aa","a"))
# print(Solution().isMatch2("aa","a*"))
# print(Solution().isMatch2("ab",".*"))
# print(Solution().isMatch2("aab","c*a*b"))
# print(Solution().isMatch2("mississippi","mis*is*p*."))
# print(Solution().isMatch2("mississippi","mis*is*ip*."))
# print(Solution().isMatch2("aaa","a*a"))  # True
# print(Solution().isMatch2("ab",".*c"))   # False


import cv2
import numpy as np

txt_file = '/home/kakaluote/2T/workspace/RVO2/output.txt'
with open(txt_file, 'r') as f:
    lines = f.readlines()

# im = np.zeros((300, 300, 3), np.uint8)

image_s = 1200
coord_scale = 2.5

obstacles = [[[-10, 40], [-40, 40], [-40, 10], [-10, 10]],
             [[10, 40], [10, 10], [40, 10], [40, 40]],
             [[10, -40], [40, -40], [40, -10], [10, -10]],
             [[-10, -40], [-10, -10], [-40, -10], [-40, -40]]]

for line in lines:
    agents = [x[1:-1].split(',') for x in line.strip().split(' ')]
    agents = [[float(x[0]), float(x[1])] for x in agents]
    # print(len(agents))

    im = np.zeros((image_s, image_s, 3), np.uint8)
    # for obstacle in obstacles:
    #     poly = [[int(x[0] * coord_scale + image_s / 2), int(x[1] * coord_scale + image_s / 2)] for x in obstacle]
    #     cv2.polylines(im,np.array([poly]),True,(0,255,0),2)

    for agent in agents:
        x, y = agent
        # print(x + 150, y + 150)
        cv2.circle(im, (int(x * coord_scale + image_s / 2), int(y * coord_scale + image_s / 2)), 2, (0, 0, 255), 2)

    cv2.imshow('demo', im)
    cv2.waitKey(0)
