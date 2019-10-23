// global variables
var nowAngle, nowDistance;
var currentRadar = 1;
var radar1_url = "http://127.0.0.1:5000/fposition";
var radar2_url = "http://127.0.0.1:5000/sposition";
var currentURL = radar1_url;

// navigation bar: change active status
$(document).ready(function(){
  $("#radar1").click(function(){
    $("#radar1").addClass("active");
    $("#radar2").removeClass("active");
    currentRadar = 1;
    currentURL = radar1_url;
    clear();
  });

  $("#radar2").click(function(){
    $("#radar2").addClass("active");
    $("#radar1").removeClass("active");
    currentRadar=2;
    currentURL = radar2_url;
    clear();
  });
});

// OVERRIDE: p5js function
function setup() {
  var cnv = createCanvas(windowWidth*0.95+31, windowHeight*1.01-50);
  background("#111");
  smooth();
}

function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
  background("#111");
  smooth();
}

// OVERRIDE: p5js funciotn
function draw() {
  fill(98,245,31);
  noStroke();
  fill(0,4);
  rect(0, 0, width, height-height*0.065);

  // get data from server
  getDataFromServer();

  // draw
  fill(98,245,31);
  drawRadarDefaultShape();
  drawSweepLine();
  drawInfoText();
  drawObject();
}

/* Get data from URL */
function getDataFromServer() {

  $.ajax({ type: "GET", url: currentURL, dataType: 'json', success: function(response){
      if (response.code == -1) {
        return;
      }
      nowAngle = float(response.angle)
      nowDistance = float(response.distance)
      //console.log(nowAngle)
      //console.log(response.time)
  } });

}

/* Drawing Radar Shape */
function drawRadarDefaultShape() {
  push();
  translate(width/2, height-height*0.074);
  noFill();

  // set line option
  strokeWeight(2);
  stroke(98, 245, 31);

  // drawing ARC
  arc(0,0,(width-width*0.0625),(width-width*0.0625),PI,TWO_PI);
  arc(0,0,(width-width*0.27),(width-width*0.27),PI,TWO_PI);
  arc(0,0,(width-width*0.479),(width-width*0.479),PI,TWO_PI);
  arc(0,0,(width-width*0.687),(width-width*0.687),PI,TWO_PI);

  // drawing line
  line(-width/2, 0, width/2, 0);
  line(0,0,(-width/2)*cos(radians(30)),(-width/2)*sin(radians(30)));
  line(0,0,(-width/2)*cos(radians(90)),(-width/2)*sin(radians(90)));
  line(0,0,(-width/2)*cos(radians(60)),(-width/2)*sin(radians(60)));
  line(0,0,(-width/2)*cos(radians(120)),(-width/2)*sin(radians(120)));
  line(0,0,(-width/2)*cos(radians(150)),(-width/2)*sin(radians(150)));
  line((-width/2)*cos(radians(30)),0,width/2,0);

  pop();
}


function drawSweepLine() {
  push();
  strokeWeight(9);
  stroke(30, 250, 60);
  translate(width/2, height-height*0.074);
  line(0, 0, (height-height*0.12)*cos(radians(nowAngle)), -(height-height*0.12)*sin(radians(nowAngle)))
  pop();
}

/* draws the texts on the screen */
function drawInfoText() {
  push();
  fill(0,0,0);
  noStroke();
  rect(0, height-height*0.0648, width, height);
  fill(98, 245, 31);

  // write distance legend and moderate position
  textSize(15);
  text("1m", width-width*0.365, height-height*0.0833);
  text("2m", width-width*0.262, height-height*0.0833);
  text("3m", width-width*0.16, height-height*0.0833);
  text("4m", width-width*0.055, height-height*0.0833);

  // write Infomation like degree
  textSize(30);
  text("Angle: " + nowAngle +" ", width-width*0.48, height-height*0.0277);
  text("Distance: ", width-width*0.26, height-height*0.0277);
  if(nowDistance<40) {
  text("        " + nowDistance +" cm", width-width*0.225, height-height*0.0277);
  }

  // write degree legend and moderate position
  textSize(25);
  fill(98, 245, 60);
  translate((width-width*0.495)+width/2*cos(radians(30)), (height-height*0.0907)- width/2*sin(radians(30)));
  rotate(-radians(-60));
  text("30",0,0)
  resetMatrix();

  translate((width-width*0.503)+width/2*cos(radians(60)),(height-height*0.0888)-width/2*sin(radians(60)));
  rotate(-radians(-30));
  text("60",0,0);
  resetMatrix();

  translate((width-width*0.507)+width/2*cos(radians(90)),(height-height*0.0833)-width/2*sin(radians(90)));
  rotate(radians(0));
  text("90",0,0);
  resetMatrix();

  translate(width-width*0.513+width/2*cos(radians(120)),(height-height*0.07129)-width/2*sin(radians(120)));
  rotate(radians(-30));
  text("120",0,0);
  resetMatrix();

  translate((width-width*0.5104)+width/2*cos(radians(150)),(height-height*0.0574)-width/2*sin(radians(150)));
  rotate(radians(-60));
  text("150",0,0);

  pop();
}

function drawObject() {
  push();
  translate(width/2,height-height*0.074);
  strokeWeight(9);
  stroke(255,10,10);

  // covers the distance
  pixsDistance = nowDistance*((height-height*0.1666)*0.025);

  // limiting the range
  if (nowDistance < 40) {
    // draws the object according to the angle and the distance
    line(pixsDistance*cos(radians(nowAngle)), -pixsDistance*sin(radians(nowAngle)),
        (width-width*0.505)*cos(radians(nowAngle)), -(width-width*0.505)*sin(radians(nowAngle)));
  }
  pop();
}
