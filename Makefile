CXX = g++
CXXFLAGS = --std=c++17 -Wall -MMD
INCLUDES = -I./libs/duckx/include -I./libs/pugixml/src -I./libs/xlsxio/include -I./libs/xlsxio/lib
LIBRARIES = -L./libs/duckx/build -L./libs/xlsxio
OBJECTS = echoparse.o ./libs/pugixml/src/pugixml.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = EchoParse

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${INCLUDES} ${LIBRARIES} ${OBJECTS} -o ${EXEC} -lduckx -lzip -lxlsxio_write

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${EXEC} ${OBJECTS} ${DEPENDS}

