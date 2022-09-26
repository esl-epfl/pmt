file(REMOVE_RECURSE
  "libpmt.pdb"
  "libpmt.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/pmt.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
