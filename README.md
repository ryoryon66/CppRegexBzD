# CppRegexBzD

Brzozowski derivativeを用いた正規表現のマッチングアルゴリズムをC++で実装したリポジトリです。微分の様子をgifファイルで保存します

# 文法

- expression ::= concat ( '|' expression )*
- concat ::= star ( concat )*
- star ::= star '*' | atom
- atom ::= char | '(' expression ')'
- char ::= [a-z]



```
#define USE_SIMPLIFICATION true
```
で簡略化を適用するかを設定できます。

# (dream|dreamer)* へのdreamdreamerの受理判定
![sample](https://github.com/ryoryon66/CppRegexBzD/assets/46624038/0f669bbd-b9f1-49c7-9cfb-b453cf62fd2a)

## (dream|dreamer|erase|eraser)\*
![image](https://user-images.githubusercontent.com/46624038/235308722-3ba2b734-76c3-416e-9a50-ed5408ed264d.png)
## (dream|dreamer|erase|eraser)\*のdによる微分。
![2023-04-29_23h44_37](https://user-images.githubusercontent.com/46624038/235308749-4bf8517b-c2c8-4447-abc5-c7978651a81b.png)
## (dream|dreamer|erase|eraser)\*のd,rによる微分。
![image](https://user-images.githubusercontent.com/46624038/235436792-a0eacbed-c863-4921-855c-6d733976db14.png)



https://dreampuf.github.io/GraphvizOnline/

にてdotファイルに吐かれたテキストをコピペして可視化を楽しむことができます。
