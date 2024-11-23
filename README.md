# linedetect_sim
opencv라이브 러리를 사용한 라인 검출 알고리즘 
헤더파일 :

![image](https://github.com/user-attachments/assets/26287b8a-4398-4d6f-ae4e-bdbc2736bb01)

코드 설명 : 
main : 

 ![image](https://github.com/user-attachments/assets/d0c0ad50-a771-4135-8b0c-60d1e0231f22)

필요한 라이브러리 및 using namespace,control + c 함수 , 자주 사용하는 상수를 디파인 해준다 .

 ![image](https://github.com/user-attachments/assets/19395696-65a7-4fa1-ac72-621f7a1f5c74)

함수 선언 및 VideoCapture,VideoWriter 객체를 선언한다.

 ![image](https://github.com/user-attachments/assets/c48a2526-30e9-4091-b00c-68669fcf0171)

함수 반복문 동영상이 끝나거나 control + c를 누를 때까지 반복한다.

suorce 로부터 영상 frame을 얻은 후 원본 파일을 writer2로 전송한다.

setFrame() 함수는 밝기처리, gray영상으로 변환 후 이진화를 수행한다..

![image](https://github.com/user-attachments/assets/e8d34dcf-5aa9-4433-a5bf-fc65b70e032a)

connectedComponentsWithStats() 함수를 통해 


 

