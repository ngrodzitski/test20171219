GroupSources(include/beast beast)

GroupSources(examples "/")

add_executable (_bench.beast.single_handler
    ${BEAST_INCLUDES}
    main.cpp
)

install(TARGETS _bench.beast.single_handler DESTINATION bin)
