CXX = g++
CXXFLAGS = --std=c++17 -Wall -MMD -O3
SUBDIRS = duckx minizip
INCLUDES = -I./libs/duckx/include -I./libs/xlsxio/include -I./libs/xlsxio/lib
LIBRARIES = -L./libs/duckx/build -L./libs/xlsxio
OBJECTS = FilePair.o EchoParse.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = EchoParse

all: $(SUBDIRS) xlsxio EchoParse
.PHONY: all

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${INCLUDES} ${LIBRARIES} ${OBJECTS} -o ${EXEC} -lduckx -lzip -lxlsxio_write

-include ${DEPENDS}


.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	cd libs/$@ && mkdir -p build && cd build && cmake .. && cmake --build .

xlsxio: minizip

xlsxio:
	cd libs/$@ && make


.PHONY: clean

clean:
	rm ${EXEC} ${OBJECTS} ${DEPENDS}

