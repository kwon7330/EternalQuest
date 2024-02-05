[프로젝트 소스코드 폴더]   
Game : 게임모드와 게임 설정에 관련된 클래스   
Gimmick : 기믹 배경 물체에 관련된 클래스   
Player : 플레이어에 관련된 클래스   
Input : 플레이어 입력에 관련된 클래스   
Character : 기본 캐릭터와 PC 및 NPC에 관련된 클래스   
Action : 캐릭터 액션에 관련된 클래스   
Item : 캐릭터 아이템에 관련된 클래스   
Stat : 캐릭터 스탯에 관련된 클래스   
AI : NPC 인공지능에 관련된 클래스   
UI : UI에 관련된 클래스   
Physics : 물리 충돌체 설정에 관련된 전처리기   
Interface : 인터페이스 클래스를 모아둔 폴더   
   
[클래스 생성]   
모든 클래스는 EQ(EternalQuest의 약자)의 접두사로 시작   
폴더마다 다른 폴더와의 의존성을 최소화   
다른 폴더의 클래스에 접근할 때 가급적 인터페이스를 통해 접근   
헤더 경로를 찾기 쉽도록 모듈.Build.cs 파일에 모듈의 폴더를 인클루드 경로 설정   
   
[최초 생성할 클래스]   
게임을 구성하는 기본 골격의 설정   
캐릭터는 PC와 NPC를 구분해서 구현   
다른 폴더의 클래스에 접근할 때 가급적 인터페이스를 통해 접근   
게임 모드에서는 입장할 플레이어의 규격을 지정(AEQPlayerController, AEQCharacterPlayer)   