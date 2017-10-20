<script language='javascript'>
	window.setTimeout('window.location.reload()',5000);
</script>

<?php 

error_reporting(E_ALL); 
ini_set('display_errors',1); 

$link= new mysqli("localhost","ntrd","test12","adudb"); 
if ($link->connect_error)  
{ 
   echo "MySQL 접속 에러 : ";
   echo mysql_connect_error();
   exit();
}
// 센서값을 SQL을 조회후 최신값을 상위로 올 수 있게 정렬
$sql="select * from iotsense order by id desc";  
$result = $link -> query($sql);

// 조회된 데이터값을 HTML태그를 사용해 표로 출력
echo "<table border=1 margin=2 width=100%>";
echo "<tr><th width=70>ID</th><th width=100>UID</th><th width=35>계급</th><th width=100>성명</th><th width=120>군번</th><th width=120>날짜/시간</th><th width=50>모드</th><th width=70>센서값</th></tr>";
if($result->num_rows >0 ){
	while($row = $result ->fetch_assoc()){
		echo "<tr>";
		
		echo "<th>";
		echo $row["id"];
		echo "</th>";
		
		echo "<th>";
		echo $row["uid"];
		echo "</th>";
		
		// UID 값을 통해 계급을 조회후 출력
		echo "<th>";
		$vtmp = $row["uid"];
		$snd_sql="select soldgrd from uid where uid like $vtmp";
		$snd_result = $link -> query($snd_sql);
		$snd_row = $snd_result ->fetch_assoc();
		echo $snd_row["soldgrd"];
		echo "</th>";
		
		// UID 값을 통해 이름을 조회후 출력
		echo "<th>";
		$vtmp = $row["uid"];
		$snd_sql="select name from uid where uid like $vtmp";
		$snd_result = $link -> query($snd_sql);
		$snd_row = $snd_result ->fetch_assoc();
		echo $snd_row["name"];
		echo "</th>";
		
		// UID 값을 통해 군번을 조회후 출력
		echo "<th>";
		$vtmp = $row["uid"];
		$snd_sql="select soldnum from uid where uid like $vtmp";
		$snd_result = $link -> query($snd_sql);
		$snd_row = $snd_result ->fetch_assoc();
		echo $snd_row["soldnum"];
		echo "</th>";
		
		echo "<th>";
		echo $row["datetime"];
		echo "</th>";
		
		// 입력된 모드값으로 종류를 구분
		echo "<th>";
		switch ( $row["mode"] ){
			case 0:
				echo "팔굽";
				break;
			case 1:
				echo "윗몸";
				break;
			case 2:
				echo "뜀걸음";
				break;
			case 3:
				echo "테스트";
				break;
			default:
				echo "알수없음";
				break;
		}
		echo "</th>";
		
		echo "<th>";
		echo $row["sense"];
		echo "</th>";
		
		echo "</tr>";
	}
}else{
	// 조회된 테이블 갯수가 0 일경우 값을 NONE으로 출력
	echo "<tr><th>NONE</th><th>NONE</th><th>NONE</th><th>NONE</th><th>NONE</th><th>NONE</th><th>NONE</th><th>NONE</th></tr>";
} 
echo "</table>";
$link -> close(); 

?>