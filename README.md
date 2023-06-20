# CppRegexBzD

Brzozowski derivativeを用いた正規表現のマッチングアルゴリズムをC++で実装したリポジトリです。微分の様子をgifファイルで保存します

# 前提ソフトウェア

```
apt -y install imagemagick
```

以上を入れなくてもdotファイルを[このサイト](https://dreampuf.github.io/GraphvizOnline/)に張り付けることでASTを可視化することが可能です。

# 使用方法

```
g++ main.cpp
./a.out [正規表現] [文字列]
```

例
```
./a.out "(dream|dreamer)*" "dreamdreamer"
```



## 文法

- expression ::= concat ( '|' expression )*
- concat ::= star ( concat )*
- star ::= star '*' | atom
- atom ::= char | '(' expression ')'
- char ::= [a-z]


また
```
#define USE_SIMPLIFICATION true
```
で簡略化を適用するかを設定できます。(簡略化を設定しないと木が大きくなる)

また出力例をexamples/以下に保存してあります。

# (dream|dreamer)* へのdreamdreamerの受理判定

![sample](https://github.com/ryoryon66/CppRegexBzD/assets/46624038/e83ec52d-37a6-49ef-a5ec-e8dbf81d3565)

## (dream|dreamer|erase|eraser)\*
![image](https://user-images.githubusercontent.com/46624038/235308722-3ba2b734-76c3-416e-9a50-ed5408ed264d.png)
## (dream|dreamer|erase|eraser)\*のdによる微分。
![2023-04-29_23h44_37](https://user-images.githubusercontent.com/46624038/235308749-4bf8517b-c2c8-4447-abc5-c7978651a81b.png)
## (dream|dreamer|erase|eraser)\*のd,rによる微分。
![image](https://user-images.githubusercontent.com/46624038/235436792-a0eacbed-c863-4921-855c-6d733976db14.png)



https://dreampuf.github.io/GraphvizOnline/

にてdotファイルに吐かれたテキストをコピペして可視化を楽しむことができます。
