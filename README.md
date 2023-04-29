# CppRegexBzD

Brzozowski derivativeを用いた正規表現のマッチングアルゴリズムをC++で実装したリポジトリです。



#define USE_SIMPLIFICATION true

#define SIMPLIFICATION_PER_ITERATION 4

で簡略化を適用するかを設定できます。

(dream|dreamer|erase|eraser)\*
![image](https://user-images.githubusercontent.com/46624038/235308722-3ba2b734-76c3-416e-9a50-ed5408ed264d.png)
(dream|dreamer|erase|eraser)\*のdによる微分。
![2023-04-29_23h44_37](https://user-images.githubusercontent.com/46624038/235308749-4bf8517b-c2c8-4447-abc5-c7978651a81b.png)

https://dreampuf.github.io/GraphvizOnline/

にてdotファイルに吐かれたテキストをコピペして可視化を楽しむことができます。
