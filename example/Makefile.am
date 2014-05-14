noinst_PROGRAMS	= \
	author \
	categories \
	concepts \
	entities \
	relations \
	feed_links \
	keywords \
	languages \
	microformats \
	text_extract \
	constraint_queries \
	parameters \
	sentiment \
	taxonomy \
	image \
	combined_data \
	keywordspp \
	entitiespp \
	relationspp

INCLUDES = -I$(top_builddir)/include -I$(top_srcdir)/include -I@srcdir@/include
DEPS = $(top_builddir)/src/libalchemyapi.la
LDADDS = $(top_builddir)/src/libalchemyapi.la -lm

author_SOURCES = author.c utils.c
author_LDFLAGS =
author_DEPENDENCIES = $(DEPS)
author_LDADD = $(LDADDS)

categories_SOURCES = categories.c utils.c
categories_LDFLAGS =
categories_DEPENDENCIES = $(DEPS)
categories_LDADD = $(LDADDS)

entities_SOURCES = entities.c utils.c
entities_LDFLAGS =
entities_DEPENDENCIES = $(DEPS)
entities_LDADD = $(LDADDS)

relations_SOURCES = relations.c utils.c
relations_LDFLAGS =
relations_DEPENDENCIES = $(DEPS)
relations_LDADD = $(LDADDS)

feed_links_SOURCES = feed_links.c utils.c
feed_links_LDFLAGS =
feed_links_DEPENDENCIES = $(DEPS)
feed_links_LDADD = $(LDADDS)

concepts_SOURCES = concepts.c utils.c
concepts_LDFLAGS =
concepts_DEPENDENCIES = $(DEPS)
concepts_LDADD = $(LDADDS)

keywords_SOURCES = keywords.c utils.c
keywords_LDFLAGS =
keywords_DEPENDENCIES = $(DEPS)
keywords_LDADD = $(LDADDS)

languages_SOURCES = languages.c utils.c
languages_LDFLAGS =
languages_DEPENDENCIES = $(DEPS)
languages_LDADD = $(LDADDS)

microformats_SOURCES = microformats.c utils.c
microformats_LDFLAGS =
microformats_DEPENDENCIES = $(DEPS)
microformats_LDADD = $(LDADDS)

text_extract_SOURCES = text_extract.c utils.c
text_extract_LDFLAGS =
text_extract_DEPENDENCIES = $(DEPS)
text_extract_LDADD = $(LDADDS)

constraint_queries_SOURCES = constraint_queries.c utils.c
constraint_queries_LDFLAGS =
constraint_queries_DEPENDENCIES = $(DEPS)
constraint_queries_LDADD = $(LDADDS)

parameters_SOURCES = parameters.c utils.c
parameters_LDFLAGS =
parameters_DEPENDENCIES = $(DEPS)
parameters_LDADD = $(LDADDS)

sentiment_SOURCES = sentiment.c utils.c
sentiment_LDFLAGS =
sentiment_DEPENDENCIES = $(DEPS)
sentiment_LDADD = $(LDADDS)

taxonomy_SOURCES = taxonomy.c utils.c
taxonomy_LDFLAGS =
taxonomy_DEPENDENCIES = $(DEPS)
taxonomy_LDADD = $(LDADDS)

image_SOURCES = image.c utils.c
image_LDFLAGS =
image_DEPENDENCIES = $(DEPS)
image_LDADD = $(LDADDS)

combined_data_SOURCES = combined_data.c utils.c
combined_data_LDFLAGS =
combined_data_DEPENDENCIES = $(DEPS)
combined_data_LDADD = $(LDADDS)

keywordspp_SOURCES = keywordspp.cpp
keywordspp_LDFLAGS =
keywordspp_DEPENDENCIES = $(DEPS)
keywordspp_LDADD = $(LDADDS)

entitiespp_SOURCES = entitiespp.cpp
entitiespp_LDFLAGS =
entitiespp_DEPENDENCIES = $(DEPS)
entitiespp_LDADD = $(LDADDS)

relationspp_SOURCES = relationspp.cpp
relationspp_LDFLAGS =
relationspp_DEPENDENCIES = $(DEPS)
relationspp_LDADD = $(LDADDS)
