file(GLOB local_source
  *.h
  *.cc
)

add_subdirectory(action)
add_subdirectory(antlr4)
add_subdirectory(common)
add_subdirectory(macro)
add_subdirectory(operator)
add_subdirectory(variable)
add_subdirectory(persistent_storage)
add_subdirectory(transformation)

set(antlr4_obj
  ${CMAKE_BINARY_DIR}/antlr4obj/ANTLRErrorListener.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ANTLRErrorStrategy.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ANTLRFileStream.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ANTLRInputStream.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ATN.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ATNConfig.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ATNConfigSet.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ATNDeserializationOptions.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ATNDeserializer.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ATNSimulator.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ATNState.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ATNStateType.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ActionTransition.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/AmbiguityInfo.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Any.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ArrayPredictionContext.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Arrays.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/AtomTransition.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/BailErrorStrategy.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/BaseErrorListener.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/BufferedTokenStream.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/CPPUtils.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/CharStream.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Chunk.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/CommonToken.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/CommonTokenFactory.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/CommonTokenStream.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ConsoleErrorListener.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ContextSensitivityInfo.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/DFA.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/DFASerializer.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/DFAState.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/DecisionEventInfo.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/DecisionInfo.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/DecisionState.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/DefaultErrorStrategy.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/DiagnosticErrorListener.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/EpsilonTransition.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ErrorInfo.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ErrorNodeImpl.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Exceptions.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/FailedPredicateException.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/InputMismatchException.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/IntStream.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/InterpreterDataReader.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/InterpreterRuleContext.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Interval.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/IntervalSet.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/IterativeParseTreeWalker.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LL1Analyzer.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Lexer.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerATNConfig.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerATNSimulator.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerAction.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerActionExecutor.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerChannelAction.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerCustomAction.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerDFASerializer.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerIndexedCustomAction.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerInterpreter.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerModeAction.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerMoreAction.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerNoViableAltException.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerPopModeAction.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerPushModeAction.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerSkipAction.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LexerTypeAction.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ListTokenSource.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/LookaheadEventInfo.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/MurmurHash.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/NoViableAltException.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/NotSetTransition.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/OrderedATNConfigSet.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ParseInfo.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ParseTree.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ParseTreeListener.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ParseTreeMatch.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ParseTreePattern.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ParseTreePatternMatcher.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ParseTreeVisitor.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ParseTreeWalker.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Parser.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ParserATNSimulator.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ParserInterpreter.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ParserRuleContext.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/PrecedencePredicateTransition.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Predicate.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/PredicateEvalInfo.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/PredicateTransition.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/PredictionContext.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/PredictionContextCache.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/PredictionContextMergeCache.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/PredictionMode.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ProfilingATNSimulator.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/ProxyErrorListener.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/RangeTransition.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/RecognitionException.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Recognizer.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/RuleContext.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/RuleContextWithAltNum.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/RuleTagToken.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/RuleTransition.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/RuntimeMetaData.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/SemanticContext.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/SetTransition.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/SingletonPredictionContext.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/StarLoopbackState.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/StringUtils.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Synchronization.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/TagChunk.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/TerminalNodeImpl.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/TextChunk.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Token.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/TokenSource.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/TokenStream.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/TokenStreamRewriter.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/TokenTagToken.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Transition.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/TransitionType.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Trees.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/UnbufferedCharStream.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/UnbufferedTokenStream.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Utf8.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/Vocabulary.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/WildcardTransition.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/WritableToken.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/XPath.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/XPathElement.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/XPathLexer.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/XPathLexerErrorListener.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/XPathRuleAnywhereElement.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/XPathRuleElement.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/XPathTokenAnywhereElement.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/XPathTokenElement.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/XPathWildcardAnywhereElement.cpp.o
  ${CMAKE_BINARY_DIR}/antlr4obj/XPathWildcardElement.cpp.o
)

add_library(wge
  $<TARGET_OBJECTS:action>
  $<TARGET_OBJECTS:antlr4>
  $<TARGET_OBJECTS:common>
  $<TARGET_OBJECTS:common_ragel>
  $<TARGET_OBJECTS:macro>
  $<TARGET_OBJECTS:operator>
  $<TARGET_OBJECTS:variable>
  $<TARGET_OBJECTS:persistent_storage>
  $<TARGET_OBJECTS:transformation>
  ${antlr4_obj}
  ${local_source}
)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(wge PRIVATE spdlog::spdlog_header_only)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  SET(antlr4_lib ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/debug/lib/libantlr4-runtime.a)
else()
  SET(antlr4_lib ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/lib/libantlr4-runtime.a)
endif()

add_custom_command(
  OUTPUT ${antlr4_obj}
  COMMAND rm -rf antlr4obj && mkdir antlr4obj && cd antlr4obj && ar -x ${antlr4_lib}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  DEPENDS antlr4
)

# add_custom_target(genexdebug COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_OBJECTS:parser>")
# cmake --build . --target genexdebug
