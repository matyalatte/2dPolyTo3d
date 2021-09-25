# 2dPolyTo3d v0.9.1
Generates a 3D model from a 2d polygon<br>
任意の多角形から3Dモデルを生成します(ラクガキ王国のアレ)

![demo](https://user-images.githubusercontent.com/69258547/133283432-dc701180-57e2-4796-9c75-4d0a10fb0ce3.gif)

## 概要
任意の多角形から3Dモデルを生成するデモです。<br>
マウスで図形を描くと3Dモデルが生成されます。<br>
windows用の実行ファイルが[こちら](https://github.com/matyalatte/2dPolyTo3d/releases)からダウンロードできます。

以下の論文を参考にしました。<br>
[五十嵐 健夫, 松岡 聡, 田中 英彦 (1999). 手書きスケッチによるモデリングシステム Teddy](https://www-ui.is.s.u-tokyo.ac.jp/~takeo/papers/siggraph99-j.pdf)<br>
[Takeo Igarashi, Satoshi Matsuoka, Hidehiko Tanaka. "Teddy:
A Sketching Interface for 3D Freeform Design”,
ACM SIGGRAPH 99, 1999. ](https://www-ui.is.s.u-tokyo.ac.jp/~takeo/papers/siggraph99.pdf)<br>
<br>
また、以下のページでアルゴリズムを解説しています。よろしければこちらもご覧ください。<br>
[手描きの丸から良い感じの3Dモデルを生成する](https://matyalatte.github.io/memo/rakugaki)

## 操作方法

- 左マウスボタン：ドラッグで図形を描画する。ボタンを離すと3Dモデルが生成される。
- 右マウスボタン：クリックでメニューを開く。
- ホイールボタン：ドラッグで3Dモデルが回転する。（ただし、重心中心ではなく原点中心の回転）

その他、キーボード操作で使用できる機能がいくつかあります。(自分用に作った機能なので、不要なものばかりだと思いますが。）<br>
詳細は[こちら](https://github.com/matyalatte/2dPolyTo3d/wiki/%E3%82%AD%E3%83%BC%E3%83%9C%E3%83%BC%E3%83%89%E6%93%8D%E4%BD%9C%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6)

## メニュー
右クリックで表示されるメニューの説明です。<br>
![menu](https://user-images.githubusercontent.com/69258547/133288791-9f3e3b54-ecc4-4fd4-b0c3-bb4a2adcb07e.png)
- Export as .obj: 生成された3Dモデルを.obj形式で出力します。
- Export as .stroke: 3Dモデルの生成に用いた多角形を[独自形式](https://github.com/matyalatte/2dPolyTo3d/wiki/.stroke%E5%BD%A2%E5%BC%8F%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6)で出力します。
- Import as .stroke: .stroke形式で保存された多角形から3Dモデルを生成します。
- Show/Hide Normal: 頂点法線ベクトルを表示/非表示します。
- Show/Hide 2D Polygon: 2Dメッシュと中心線を表示/非表示します。
- Show/Hide Model: 3Dモデルを表示/非表示します
- EXIT: アプリを終了します。

## 注意事項等
- 今回初めてc++を触りました。ポインタの扱い等、多少稚拙な部分があるかもしれません。
- 稀にアクセス違反で強制終了します。
- OpenGL用のNuGetパッケージを使用しています。詳細はpackage.configを参照。
