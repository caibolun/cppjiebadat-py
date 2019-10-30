#include "gtest/gtest.h"
#include "cppjieba/PreFilter.hpp"
#include "limonp/StringUtil.hpp"

using namespace cppjieba;
using namespace std;

inline string GetStringFromRunes(const string& s, RuneStrArray::const_iterator left, RuneStrArray::const_iterator right) {
  assert(right->offset >= left->offset);
  uint32_t len = right->offset - left->offset + right->len;
  return s.substr(left->offset, len);
}

TEST(PreFilterTest, Test1) {
  unordered_set<Rune> symbol;
  symbol.insert(65292u); // "，"
  symbol.insert(12290u); // "。"
  string expected;
  string res;

  {
    string s = "你好，美丽的，世界";
    PreFilter filter(symbol, s);
    expected = "你好/，/美丽的/，/世界";
    ASSERT_TRUE(filter.HasNext());
    vector<string> words;
    while (filter.HasNext()) {
      auto range = filter.Next();
      words.push_back(GetStringFromRunes(s, range.left, range.right - 1));
    }
    res = limonp::Join(words.begin(), words.end(), "/");
    ASSERT_EQ(res, expected);
  }

  {
    string s = "我来自北京邮电大学。。。学号123456，用AK47";
    PreFilter filter(symbol, s);
    expected = "我来自北京邮电大学/。/。/。/学号123456/，/用AK47";
    ASSERT_TRUE(filter.HasNext());
    vector<string> words;
    while (filter.HasNext()) {
      auto range = filter.Next();
      words.push_back(GetStringFromRunes(s, range.left, range.right - 1));
    }
    res = limonp::Join(words.begin(), words.end(), "/");
    ASSERT_EQ(res, expected);
  }
}

