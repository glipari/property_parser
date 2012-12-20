AM_CPPFLAGS=-Wall -std=c++0x -I${top}
ACLOCAL_AMFLAGS=-I m4

lib_LTLIBRARIES = libproperty_parser.la
libproperty_parser_la_SOURCES = property_parser.cpp exceptions.cpp \
	 property_visitor.cpp string_utils.cpp property.cpp

noinst_PROGRAMS = example

example_SOURCES = example.cpp 
example_LDADD = -lproperty_parser

