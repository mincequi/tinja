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

# Performance
To evaluate performance there is a [benchmark](test/source/benchmark.cpp) provided.
Comparison is done with several other template engines:
* [Micro Moustache](https://github.com/rzeldent/micro-moustache)
* [Kainjow Mustache](https://github.com/kainjow/Mustache)
* [Inja](https://github.com/pantor/inja)
* [Bustache](https://github.com/jamboree/bustache)

There are multiple test scenarios to try out:
* *cold* overall time of **parse once** and **render once**
* *preprocessed* time to **render basic template** (taking out parsing step)
* *arrays* time to **render complex template** (with loops)

```
...............................................................................
cold                                       samples    iterations          mean
-------------------------------------------------------------------------------
micro_mustache                                 100             1    25.5542 us
kainjow_mustache                               100             1    147.675 us
inja                                           100             1    37.9807 us
bustache                                       100             2    13.0128 us
tinja                                          100             8    3.40686 us
tinja --32_nodes_reserved                      100             9    2.83972 us
tinja --concat                                 100             7    3.92889 us
tinja --32_nodes_reserved --concat             100             8    3.34835 us
  
-------------------------------------------------------------------------------
preparsed                                  samples    iterations          mean
-------------------------------------------------------------------------------
micro_mustache                                 100             1    25.9838 us
kainjow_mustache                               100             3    10.9406 us
inja                                           100             5    6.13677 us
bustache                                       100            11    2.30991 us
tinja                                          100            22    1.08674 us
tinja --concat                                 100            16    1.57428 us

-------------------------------------------------------------------------------
arrays                                     samples    iterations          mean
-------------------------------------------------------------------------------
kainjow_mustache                               100             1     60.323 us
inja                                           100             1    111.585 us
bustache --concat --nlohmann                   100             2    23.8236 us
tinja                                          100             2    16.2217 us
tinja --concat                                 100             2    19.8571 us
```
Lower is better.

*Micro Mustache* performs quite nice despite its simple implementation but cannot preparse nor loop.
*Kainjow Mustache* shows bad parsing performance but outperforms *inja* when it comes to looping.
*Inja* shows nice performance only for simple templates.
*Bustache* has the best performance when it comes to a full-featured *mustache* implementation.
*Tinja* outperforms *Bustache* by a factor of **1.2** to **4.6** depending on the scenario but does not offer a complete *mustache* implementation.

# Licensing
[![GNU GPLv3 Image](https://www.gnu.org/graphics/gplv3-127x51.png)][2]  
**Tinja** is Free Software: You can use, study, share and improve it at your
will. Specifically you can redistribute and/or modify it under the terms of the
[GNU General Public License][3] as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
