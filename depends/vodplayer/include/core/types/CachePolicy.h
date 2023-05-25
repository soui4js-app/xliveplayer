#pragma once

namespace transvod {

enum CachePolicy {
  CachePolicy_NoCache = 0,
  CachePolicy_Whole_File_Content = 1,
  CachePolicy_Stream_Latest_Gop = 2,
};

} // namespace transvod
