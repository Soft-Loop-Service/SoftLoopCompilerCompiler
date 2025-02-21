# Soft Loop Compiler Compiler (SLCC)

各プロジェクトでそれぞれ改良を重ねていたコンパイラコンパイラを開発効率化のため集約しました。
このリポジトリをサブリポジトリとして利用できます。


## SLCCの目玉機能
SLCC 独自拡張BNFでできる記法

### 1.バーティカルバー
右辺のどちらかを選択

`<E> ::= "a" | "b"`

### 2.字句解析で取得したラベルを基にした構文解析
`TEXT` `NUM` `TYPE` `DEFINITION`

### 3.アスタリスク
1回以上の繰り返し

`<E> ::= <number> * ` </br>
`<number> ::= TEXT`
機能2と組み合わせることで、これだけで任意の個数の文字列 (例 : `HELLO` `WORLD`) を受理する構文定義ができます。

### 4.左再帰の除去
構文解析でやっかいな左再帰の構文定義をしても、自動的に除去、解析できます。アイテム集合の生成過程で除去を実現

