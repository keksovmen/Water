


function del(fileName){
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function(){
		if(this.readyState == 4 && this.status == 200){
			window.location.reload(false);
		}
	}
	
	
	xhttp.open("Get", "Delete.php?file=" + fileName, true);
	xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xhttp.send();
	
};


function sendTemperatures(){
	var tempUp = document.getElementById("tu").value;
	var tempDown = document.getElementById("td").value;
	
	if(tempUp == "" || tempDown == ""){
		return;
	}
	
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function(){
		if(this.readyState == 4 && this.status == 200){
			window.location.reload(false);
		}
	}
	
	
	xhttp.open("Post", "AddTime.php", true);
	xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xhttp.send("tu=" + tempUp + "&td=" + tempDown);
}

