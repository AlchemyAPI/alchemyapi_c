=============================
AlchemyAPI C SDK: version 0.9
=============================

This is a C implementation of the AlchemyAPI SDK.  C++ API wrappers are also
included with this distribution.


INSTALLATION

1. ./autobuild.sh

2. ./configure 
    (Use any configure options you desire here)

3. make

4. make install


RUNNING EXAMPLES

Several code examples are included to illustrate using the AlchemyAPI
for named entity extraction, text classification, language identification,
and other tasks.

All code samples are within the "example" directory.

To run these code samples you must first edit the (example/data/api_key.txt) file, 
adding your assigned AlchemyAPI key.

Compiling Code Samples:

   1. See INSTALLATION section.

Running Code Samples: (first enter the "examples" directory)

   1. Entity Extraction: ./entities

   2. Concept Tagging: ./concepts

   3. Keyword Extraction: ./keywords

   4. Text Categorization: ./categories

   5. Language Identification: ./language

   6. HTML Text Extraction: ./text_extract

   7. HTML Structured Content Scraping: ./constraint_queries

   8. Microformats Extraction: ./microformats

   9. RSS / ATOM Feed Links Extraction: ./feed_links
 
  10. Sentiment Analysis: ./sentiment

  11. Relations Extraction: ./relations

  12. Author Extraction: ./author

  13. Taxonomy: ./taxonomy

  14. Image Extraction: ./image

  15. Combined Call: ./combined

DEPENDENCIES

This module requires a gcc compiler, libcurl and libxml2.


COPYRIGHT AND LICENCE

Copyright (C) 2014 AlchemyAPI, LLC.

This library is free software; you can redistribute it and/or modify
under the terms of LGPL, see COPYING file for more information.
