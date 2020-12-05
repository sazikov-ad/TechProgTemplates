//
// Created by akhtyamovpavel on 5/1/20.
//


#include "TreeTestCase.h"
#include "Tree.h"

#include <unordered_set>

#define TREE_TEST(NAME) TEST_F(TreeTestCase, NAME)

TREE_TEST(DirsAreEqual) {
  FileNode a{"foo", false, {}};
  FileNode b{"foo", false, {}};
  
  FileNode c{"bar", true, {FileNode{"foo", false, {}}}};
  FileNode d{"bar", true, {FileNode{"foo", false, {}}}};

  ASSERT_TRUE(a == b);
  ASSERT_TRUE(c == d);
}

TREE_TEST(DirsAreNotEqual) {
  FileNode a{"foo", false, {}};
  FileNode b{"foo", true, {}};
  FileNode c{"foo", true, {FileNode{"bar", false, {}}}};
  FileNode d{"bar", false, {}};
  
  ASSERT_FALSE(a == b);
  ASSERT_FALSE(a == c);
  ASSERT_FALSE(a == d);
  
  ASSERT_FALSE(b == c);
  ASSERT_FALSE(b == d);
  
  ASSERT_FALSE(c == d);
}

template <class P>
bool CheckTree(const FileNode* node, P predicate) {
  bool result = predicate(*node);
  for(const auto& c : node->children) {
    result &= CheckTree(&c, predicate);
  }
  return result;
}

TREE_TEST(GetTreeDirsOnly) {
  auto node = GetTree("/boot", true);

  ASSERT_EQ(node.name, "boot");
  ASSERT_TRUE(CheckTree(&node, [](const FileNode& n) { return n.is_dir; }));
}

TREE_TEST(GetTree) {
  std::unordered_set<std::string> headers = {"stdlib.h", "stdio.h", "time.h", "unistd.h"};
  std::unordered_set<std::string> sys_headers = {"fcntl.h", "stat.h", "time.h", "xattr.h", "types.h"};
  auto node = GetTree("/usr/include", false);

  int files_found = 0;
  int sys_files_found = 0;

  for(const auto& c : node.children) {
    if(!c.is_dir && headers.count(c.name) == 1) {
      ++files_found;
    } else if(c.name == "sys") {
      for(const auto& sc : c.children) {
        if(sys_headers.count(sc.name) == 1) {
          ++sys_files_found;
        }
      }
    }
  }

  ASSERT_EQ(files_found, headers.size());
  ASSERT_EQ(sys_files_found, sys_headers.size());
}

TREE_TEST(PassNotDirectory) {
  ASSERT_ANY_THROW(GetTree("/usr/include/stdlib.h", false));
}

TREE_TEST(PassInvalidPath) {
  ASSERT_ANY_THROW(GetTree("//bar////sdf/ sdj. ksd. jksd+\\", true));
}
