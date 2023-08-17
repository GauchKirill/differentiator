//      flag         name          discription                                          function
DEF_MOD("-d",   DIFFERENTIATOR,     "взятие производной любого порядка",    mode_differ(equation_file))
DEF_MOD("-t",   TEILOR,             "разложение в ряд тейлора",             mode_tailor(equation_file))
DEF_MOD("-c",   CALCULATOR,         "вычисление/упрощение выражения",       calculator(equation_file))
DEF_MOD("-h",   HELP,               "вывод все возможные режимы",           help())
DEF_MOD("-e",   EXIT,               "выход из программы",                   return)