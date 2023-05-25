import * as soui4 from "soui4";

function onInit(e){
	soui4.log("onInit");
}

function onExit(e){
	soui4.log("onExit");
}

function onTest(){
	soui4.SMessageBox(soui4.GetActiveWindow(),"onTest","qjs",0);
}

globalThis.onInit=onInit;
globalThis.onExit=onExit;
globalThis.onTest=onTest;