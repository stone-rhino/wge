grammar SecLang;
sec_action: 'Sec' ID LIST;
ID: [a-z]+;
LIST: [a-z]+;
WS: [ \t\r\n]+ -> skip;