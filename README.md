# tinja

Tinja is a tiny template engine for C++17, loosely inspired by 
[inja](https://github.com/pantor/inja) and [mustache](https://mustache.github.io). It is 
"logic-less" because no complex statements within your templates are needed. Instead, there 
are only two kind of tags: **variables** `{{name}}` and **arrays** `{[Hello {{name}}!]}` 
which fulfill most needs for anyone.

# Features
* Single header only
* Zero dependencies
* Logic less
* Super fast (outperforms any other template engine)
* Highly memory efficient (runs on ESP8266)

```.cpp
#include <tinja.hpp>

tinja::Template templ("Hello {{name}}!");
tinja::DataMap data;
data["name"] = "world";
auto tokens = templ.render(data); // Returns "Hello world!"

templ.parse("{[Hello {{others}}!]}");
data["others"] = tinja::Strings { "Mike", "Charly", "Leo" };
tokens = doc.render(data); // Returns "Hello Mike!", "Hello Charly!", "Hello Leo!"
```

# Building and installing

Tinja is a single header library, which can be downloaded directly from the `include/` folder.

# Licensing

[![GNU GPLv3 Image](https://www.gnu.org/graphics/gplv3-127x51.png)][2]  

**Tinja** is Free Software: You can use, study, share and improve it at your
will. Specifically you can redistribute and/or modify it under the terms of the
[GNU General Public License][3] as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
