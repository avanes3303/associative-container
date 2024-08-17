# Binary Search Tree

STL-совместимый контейнер, представляющий из себя шаблон, параметрезируемый типом хранимых объетов, оператором сравнения и аллокатором.

## Описание

Реализован в виде бинарного дерева поиска, с помощью [Tag Dispatch Idiom](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Tag_Dispatching) реализованы различные [способы обхода дерева (in-, pre-, post-order)](https://en.wikipedia.org/wiki/Tree_traversal) через итератор.

Удовлетворяет требованиям:
- [контейнера](https://en.cppreference.com/w/cpp/named_req/Container)
- [ассоциативный контейнера](https://en.cppreference.com/w/cpp/named_req/AssociativeContainer)
- [контейнера с обратным итератором](https://en.cppreference.com/w/cpp/named_req/ReversibleContainer)
- [контейнера, поддерживающего аллокатор](https://en.cppreference.com/w/cpp/named_req/AllocatorAwareContainer)
- [oбладает двунаправленным итератором](https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator)

Покрыт тестами с помощью фреймворка [Google Test](http://google.github.io/googletest).
