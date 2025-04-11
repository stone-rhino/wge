#pragma once

// Removes common comments chars (/*, */, --, #).
%%{
  machine remove_comments_char;
  
  action skip {}

  action exec_transformation { 
    result.resize(input.size());
    r = result.data();
    if(ts > input.data()){
      memcpy(r, input.data(), ts - input.data());
      r += ts - input.data();
    }
    p = ts;
    fhold;
    fgoto transformation;
  }

  # prescan
  main := |*
    '--' => exec_transformation;
    '#' => exec_transformation;
    '/*' => exec_transformation;
    '*/' => exec_transformation;
    '<!--' => exec_transformation;
    any => skip;
  *|;
  
  transformation := |*
    '--' => skip;
    '#' => skip;
    '/*' => skip;
    '*/' => skip;
    '<!--' => skip;
    any => { *r++ = fc; };
  *|;
}%%

%% write data;

static bool removeCommentsChar(std::string_view input, std::string& result) {
  result.clear();
  char* r = nullptr;

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char* ts, *te;
  int cs,act;

  %% write init;
  %% write exec;

  if(r) {
    result.resize(r - result.data());
    return true;
  }

  return false;
}