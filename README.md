[Watch this video](https://www.youtube.com/watch?v=yIgvB6UW17A)

<br>


**WinApi와 C++로 제작한 컴퓨터와 두는 체스 게임입니다.**

마우스의 드래그앤 드랍을 동작합니다.

게임 프레임워크를 연습하며 만든거라 없어도 되는 클래스들도 존재합니다.

<br>

AI의 로직에는 Minimax 알고리듬과 알파-베타 가지치기 기법정도만 쓰였습니다.

사람과 두는 거라 Iterative Deepening 과 같은 기법은 사용하지 않았습니다.

(저와 같은 체스 초보자에게는 이 정도 재귀 Depth레벨로 수를 찾아도 충분하더군요..)

<br>

Easy, Normal, Hard 모드가 있고

각 모드에서 AI가 재귀로 들어가는 Depth레벨에 차이가 있습니다.

앙파상, 캐슬링 같은 룰은 구현하지 않았고

기본 체스 룰에 프로모션은 구현되어 있습니다.

<br>

핵심 로직들은

Manager 폴더의 Devscene  

Object 폴더의 board, AI, Human, Pieces 쪽에 모여있습니다.

<br>
<br>
  

**오른쪽 버튼 기능**

Toggle Show Can Go - 피스를 클릭하고 있을 때 그 피스가 이동할 수 있는 칸을 보여줍니다 (토글 on/off)

Toggle Show Attack Zone - AI가 다음턴에 공격할 수 있는 칸들을 보여줍니다 (토글 on/off)

Move Back - 방금 전 한 수 취소하기 (연속으로 누르면 연속으로 적용됩니다)
