# tinja

Tinja is a tiny template engine for C++17, loosely inspired by [inja](https://github.com/pantor/inja) and [mustache](https://mustache.github.io). It is "logic-less" because no complex statements within your template are needed. Instead there are only two kind of tags: variables {{name}} and arrays {[Hello {{name}}!]} which will fulfill most needs.


## Features
* Single header only
* Zero dependencies
* Logic less
* Super fast (outperforms any other template engine)
* Highly memory efficient (runs on ESP8266)


```.cpp
#include <tinja.hpp>

tinja::Document doc("Hello {{name}}!");
tinja::DataMap data;
data["name"] = "world";
auto result = doc.render(data); // Returns "Hello world!"

doc.parse("{[Hello {{others}}!]}");
data["others"] = std::vector<std::string>> { "Mike", "Charly", "Leo" };
result = doc.render(data); // Returns "Hello Mike!", "Hello Charly!", "Hello Leo!"
```
