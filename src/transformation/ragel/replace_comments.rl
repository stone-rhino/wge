#pragma once

%%{
  machine replace_comments;

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
    '/*' => exec_transformation;
    any => skip;
  *|;

  # transformation
  transformation := |*
    '/*' => { *r++ = ' '; fgoto replace; };
    any => { *r++ = fc; };
  *|;

  replace := |*
    '*/' => { fgoto transformation; };
    any => skip;
  *|;
}%%

%% write data;

static bool replaceComments(std::string_view input, std::string& result) {
  result.clear();
  char* r = nullptr;

  const char* p = input.data();
  const char* ps = p;
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