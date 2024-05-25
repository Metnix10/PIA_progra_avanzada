#include <windows.h>
#include <fstream>
#include "resource.h"
#include <string.h>
#include <CommCtrl.h>
#include <CommDlg.h>
#include <iostream>

using namespace std;

struct fecha
{
	int anio;
	int mes;
	int dia;
};
struct empleados {
	empleados* ant = nullptr;
	char nombre[30];
	char rfc[15];
	char clave[12];
	char contrasenia[10];
	empleados* sig = nullptr;
	char foto[MAX_PATH];
};

empleados* auxemp = nullptr;
empleados* inicioemp = nullptr;
empleados* finemp = nullptr;
empleados* datos = nullptr;


struct salas {
	char idsal[10];
	salas* ant = nullptr;
	int asientostot;
	salas* sig = nullptr;
};

salas* auxsal = nullptr;
salas* iniciosal = nullptr;
salas* finsal = nullptr;

struct funciones {
	funciones* ant = nullptr;
	fecha fechafunc;
	char salas[10];
	char idfunc[10];
	char nombrepel[60];
	int duracion;
	char clasif[5];
	char idioma[12];
	int preciobol;
	int asientosdisp;
	char hora[10];
	funciones* sig = nullptr;
	char poster[MAX_PATH];
};

funciones* auxfunc = nullptr;
funciones* iniciofunc = nullptr;
funciones* finfunc = nullptr;

struct ventas {
	ventas* ant = nullptr;
	fecha fechaventa;
	int venta;
	char tipodepago[15];
	char poster1[MAX_PATH];
	ventas* sig = nullptr;
};

ventas* auxvent = nullptr;
ventas* iniciovent = nullptr;
ventas* finvent = nullptr;

void regEmp();
void elimEmp();

void regSala();
void elimSala();

void regFunc();
void elimFunc();

void venta();

void escribir();
void leer();

bool validarrfc(HWND hwndthis, int IDEDIT);
bool validarletras(HWND hwndthis, int IDEDIT);

HINSTANCE global;
HBITMAP imagen;

LRESULT CALLBACK ProcLogin(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProcRegSala(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProcRegEmp(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProcRegFunc(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProcListEmp(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProcListSala(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProcListFunc(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProcRepVentas(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProcVentas(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam);
char zFile[MAX_PATH] = { 0 };


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hinstprev, LPSTR cmdline, int cmdshow)
{

	leer();
	HWND PointLogin = CreateDialog(hinstance, MAKEINTRESOURCE(login), NULL, ProcLogin);

	HMENU Menu = LoadMenu(hinstance, MAKEINTRESOURCE(IDR_MENU1));
	SetMenu(PointLogin, Menu);

	global = hinstance;

	ShowWindow(PointLogin, cmdshow);

	MSG mensajes;
	while (GetMessage(&mensajes, 0, 0, 0)) {
		TranslateMessage(&mensajes);
		DispatchMessageW(&mensajes);
	}
	return 0;
}

LRESULT CALLBACK ProcLogin(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: {
		switch (wParam) {
		case IDOK: {
			char claveini[50] = { 0 };
			GetDlgItemText(hwndthis, IDC_EDIT1, claveini, 50);
			auxemp = inicioemp;
			while (auxemp != NULL and strcmp(claveini, auxemp->clave) != 0)
			{
				auxemp = auxemp->sig;
			}
			if (auxemp == NULL) {
				MessageBox(hwndthis, "Usuario no encontrado", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {
				char password[50] = { 0 };
				GetDlgItemText(hwndthis, IDC_EDIT2, password, 50);
				if (strcmp(auxemp->contrasenia, password) == 0) {
					OutputDebugString(auxemp->nombre);
					OutputDebugString("\n");

					datos = auxemp;
					EndDialog(hwndthis, 0);
					DialogBox(global, MAKEINTRESOURCE(ventvent), NULL, ProcVentas);
					return TRUE;
				}
				else {
					MessageBox(hwndthis, "Contrasenia no encontrada", "AVISO", MB_OK | MB_ICONERROR);
				}
			}
		}break;
		}
	}break;
	}
	return 0;
}
LRESULT CALLBACK ProcRegSala(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		HBITMAP foto = (HBITMAP)LoadImage(NULL, datos->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		SendDlgItemMessage(hwndthis, foto2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);


		if (datos != nullptr) {
			SetDlgItemText(hwndthis, nombre2, datos->nombre);
		}
	}break;
	case WM_COMMAND: {
		switch (wParam) {
			case ID_VER_EMPLEADOS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listemp), hwndthis, ProcListEmp);
			}break;
			case ID_REGISTRAR_SALAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(regsal), hwndthis, ProcRegSala);
			}break;
			case ID_VER_SALAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listsal), hwndthis, ProcListSala);
			}break;
			case ID_REGISTRAR_FUNCIONES: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(regfunc), hwndthis, ProcRegFunc);
			}break;
			case ID_REGISTRAR_EMPLEADOS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(ventregemp), hwndthis, ProcRegEmp);
			}break;
			case ID_VER_FUNCIONES: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listfunc), hwndthis, ProcListFunc);
			}break;
			case ID_VENTAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(ventvent), hwndthis, ProcVentas);
			}break;
			case ID_REPORTEDEVENTAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(repvent), hwndthis, ProcRepVentas);
			}break;
		case IDOK: {
			auxsal = new salas;
			regSala();
			GetDlgItemText(hwndthis, numsal, auxsal->idsal, sizeof(auxsal->idsal));
			auxsal->asientostot = GetDlgItemInt(hwndthis, IDC_EDIT3, nullptr, false);

			escribir();
			MessageBox(hwndthis, "Sala registrada", "Procedimiendo completo", MB_OK | MB_ICONINFORMATION);
		}break;
		}
	}break;
	}
	return 0;
}
LRESULT CALLBACK ProcRegEmp(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: {
		switch (wParam) {
			case ID_VER_EMPLEADOS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listemp), hwndthis, ProcListEmp);
			}break;
			case ID_REGISTRAR_SALAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(regsal), hwndthis, ProcRegSala);
			}break;
			case ID_VER_SALAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listsal), hwndthis, ProcListSala);
			}break;
			case ID_REGISTRAR_FUNCIONES: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(regfunc), hwndthis, ProcRegFunc);
			}break;
			case ID_REGISTRAR_EMPLEADOS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(ventregemp), hwndthis, ProcRegEmp);
			}break;
			case ID_VER_FUNCIONES: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listfunc), hwndthis, ProcListFunc);
			}break;
			case ID_VENTAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(ventvent), hwndthis, ProcVentas);
			}break;
			case ID_REPORTEDEVENTAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(repvent), hwndthis, ProcRepVentas);
			}break;
		case subimg: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwndthis;
			ofn.lpstrFilter = "Bitmaps (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 1;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP fotoai;
				fotoai = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
				if (fotoai != NULL) {
					SendDlgItemMessage(hwndthis, regfoto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fotoai);
				}
				else {
					MessageBox(hwndthis, "Error al cargar la imagen", "Error", MB_OK | MB_ICONERROR);
				}
			}
			else {
				MessageBox(hwndthis, "Seguro que desea cancelar la seleccion del archivo?", "Aviso", MB_OKCANCEL | MB_ICONQUESTION);

			}

		}break;
		case IDOK: {
			bool flag = validarrfc(hwndthis, rfcemp);
			bool flag2 = validarletras(hwndthis, nomemp);
			if (flag and flag2) {
				auxemp = new empleados;
				regEmp();
				strcpy_s(auxemp->foto, zFile);
				GetDlgItemText(hwndthis, nomemp, auxemp->nombre, sizeof(auxemp->nombre));
				GetDlgItemText(hwndthis, rfcemp, auxemp->rfc, sizeof(auxemp->rfc));
				GetDlgItemText(hwndthis, claveemp, auxemp->clave, sizeof(auxemp->clave));
				GetDlgItemText(hwndthis, contemp, auxemp->contrasenia, sizeof(auxemp->contrasenia));

				escribir();
				MessageBox(hwndthis, "Empleado registrado", "Procedimiendo completo", MB_OK | MB_ICONINFORMATION);

			}
		}break;
		}
		}break;
	}
	return 0;
}
LRESULT CALLBACK ProcRegFunc(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam) { 
	switch(msg){
	case WM_INITDIALOG: {
		HBITMAP foto = (HBITMAP)LoadImage(NULL, datos->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		SendDlgItemMessage(hwndthis, foto4, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);

		if (datos != nullptr) {
			SetDlgItemText(hwndthis, nombre4, datos->nombre);
		}
			char cla[5][2] = { "A", "B", "C", "D", "E" };
			for (int i = 0; i < 5; i++) {
				SendDlgItemMessage(hwndthis, IDC_COMBO1, CB_INSERTSTRING, (WPARAM)i, (LPARAM)cla[i]);
			}
			char hora[12][6] = { "9:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00", "17:00", "18:00", "19:00", "20:00" };
			for (int i = 0; i < 12; i++) {
				SendDlgItemMessage(hwndthis, IDC_COMBO4, CB_INSERTSTRING, (WPARAM)i, (LPARAM)hora[i]);
			}
			char idi[2][10] = { "original", "doblado" };
			for (int i = 0; i < 2; i++) {
				SendDlgItemMessage(hwndthis, IDC_COMBO3, CB_INSERTSTRING, (WPARAM)i, (LPARAM)idi[i]);
			}
			int ind = 0;
			auxsal = iniciosal;
			while (auxsal != NULL) {
				SendDlgItemMessage(hwndthis, IDC_COMBO2, CB_INSERTSTRING, (WPARAM)ind, (LPARAM)auxsal->idsal);
				auxsal = auxsal->sig;
				ind++;
			}
	}break;
	case WM_COMMAND: {
		switch (LOWORD (wParam)) {
			case ID_VER_EMPLEADOS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listemp), hwndthis, ProcListEmp);
			}break;
			case ID_REGISTRAR_SALAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(regsal), hwndthis, ProcRegSala);
			}break;
			case ID_VER_SALAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listsal), hwndthis, ProcListSala);
			}break;
			case ID_REGISTRAR_FUNCIONES: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(regfunc), hwndthis, ProcRegFunc);
			}break;
			case ID_REGISTRAR_EMPLEADOS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(ventregemp), hwndthis, ProcRegEmp);
			}break;
			case ID_VER_FUNCIONES: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listfunc), hwndthis, ProcListFunc);
			}break;
			case ID_VENTAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(ventvent), hwndthis, ProcVentas);
			}break;
			case ID_REPORTEDEVENTAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(repvent), hwndthis, ProcRepVentas);
			}break;
			
		case IDC_BUTTON1: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwndthis;
			ofn.lpstrFilter = "Bitmaps (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 1;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP fotoai;
				fotoai = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
				if (fotoai != NULL) {
					SendDlgItemMessage(hwndthis, bitmap11, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fotoai);
				}
				else {
					MessageBox(hwndthis, "Error al cargar la imagen", "Error", MB_OK | MB_ICONERROR);
				}
			}
			else {
				MessageBox(hwndthis, "Seguro que desea cancelar la seleccion del archivo?", "Aviso", MB_OKCANCEL | MB_ICONQUESTION);

			}

		}break;
		case IDOK: {
			/*bool diferente = true;
			auxfunc = iniciofunc;
				while (auxfunc != nullptr) {
					char Text[60] = { 0 };
					GetDlgItemText(hwndthis, IDC_EDIT12, Text, 100);
					if (strcmp(Text, auxfunc->idfunc) == 0) {
						diferente = false;
					}
				}
			if (diferente){*/
			auxfunc = new funciones;
			regFunc();
			GetDlgItemText(hwndthis, IDC_EDIT12, auxfunc->idfunc, sizeof(auxfunc->idfunc));
			GetDlgItemText(hwndthis, IDC_EDIT1, auxfunc->nombrepel, sizeof(auxfunc->nombrepel));
			SendDlgItemMessage(hwndthis, IDC_COMBO1, WM_GETTEXT, 128, (LPARAM)auxfunc->clasif);
			SendDlgItemMessage(hwndthis, IDC_COMBO2, WM_GETTEXT, 128, (LPARAM)auxfunc->salas);
			SendDlgItemMessage(hwndthis, IDC_COMBO3, WM_GETTEXT, 128, (LPARAM)auxfunc->idioma);
			SendDlgItemMessage(hwndthis, IDC_COMBO4, WM_GETTEXT, 128, (LPARAM)auxfunc->hora);
			auxfunc->duracion = GetDlgItemInt(hwndthis, IDC_EDIT2, nullptr, false);
			auxfunc->preciobol = GetDlgItemInt(hwndthis, IDC_EDIT9, nullptr, false);
			SYSTEMTIME fechaSeleccionada = { 0 };
			ZeroMemory(&fechaSeleccionada, sizeof(fechaSeleccionada));
			SendDlgItemMessage(hwndthis, IDC_DATETIMEPICKER1, DTM_GETSYSTEMTIME, 0, (LPARAM)&fechaSeleccionada);
			auxfunc->fechafunc.dia = fechaSeleccionada.wDay;
			auxfunc->fechafunc.mes = fechaSeleccionada.wMonth;
			auxfunc->fechafunc.anio = fechaSeleccionada.wYear;
			strcpy_s(auxfunc->poster, zFile);
			auxsal = iniciosal;
			while (auxsal != NULL) {
				if (strcmp(auxfunc->salas, auxsal->idsal) == 0) {
					auxfunc->asientosdisp = auxsal->asientostot;
				}
				auxsal = auxsal->sig;
			}
			escribir();
			MessageBox(hwndthis, "Funcion registrada", "Procedimiendo completo", MB_OK | MB_ICONINFORMATION);
			/*}*/
			/*else {
				MessageBox(hwndthis, "Error", "Procedimiendo incompleto", MB_OK | MB_ICONINFORMATION);
			}*/
		}break;
		}
	}break;
}
	return 0;
}
LRESULT CALLBACK ProcListSala(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam) {
	char Text[60] = { 0 };
	switch (msg) {
	case WM_INITDIALOG: {
		HBITMAP foto = (HBITMAP)LoadImage(NULL, datos->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		SendDlgItemMessage(hwndthis, foto3, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);


		if (datos != nullptr) {
			SetDlgItemText(hwndthis, nombre3, datos->nombre);
		}
		auxsal = iniciosal;
		SendDlgItemMessage(hwndthis, listboxsal, LB_SETCURSEL, 0, 0);
		while (auxsal != nullptr) {
			SendMessage(GetDlgItem(hwndthis, listboxsal), LB_INSERTSTRING, 0, (LPARAM)auxsal->idsal);
			auxsal = auxsal->sig;
		}
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_VER_EMPLEADOS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(listemp), hwndthis, ProcListEmp);
		}break;
		case ID_REGISTRAR_SALAS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(regsal), hwndthis, ProcRegSala);
		}break;
		case ID_VER_SALAS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(listsal), hwndthis, ProcListSala);
		}break;
		case ID_REGISTRAR_FUNCIONES: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(regfunc), hwndthis, ProcRegFunc);
		}break;
		case ID_REGISTRAR_EMPLEADOS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(ventregemp), hwndthis, ProcRegEmp);
		}break;
		case ID_VER_FUNCIONES: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(listfunc), hwndthis, ProcListFunc);
		}break;
		case ID_VENTAS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(ventvent), hwndthis, ProcVentas);
		}break;
		case ID_REPORTEDEVENTAS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(repvent), hwndthis, ProcRepVentas);
		}break;

		case IDCANCEL:
		{
			char elim[100] = { 0 };
			int setlist = SendDlgItemMessage(hwndthis, listboxsal, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwndthis, listboxsal, LB_GETTEXT, setlist, (LPARAM)elim);

			auxsal = iniciosal;
			while (auxsal != NULL && strcmp(auxsal->idsal, elim) != 0) {
				auxsal = auxsal->sig;
			}
			if (auxsal != NULL) {

				elimSala();
				SendMessage(GetDlgItem(hwndthis, listboxsal), LB_RESETCONTENT, 0, 0);

				auxsal = iniciosal;
				SendDlgItemMessage(hwndthis, listboxsal, LB_SETCURSEL, 0, 0);
				while (auxsal != nullptr) {
					SendMessage(GetDlgItem(hwndthis, listboxsal), LB_INSERTSTRING, 0, (LPARAM)auxsal->idsal);
					auxsal = auxsal->sig;
				}

				SetDlgItemText(hwndthis, IDC_EDIT1, "");

				escribir();
				MessageBox(hwndthis, "Sala eliminada", "Procedimiento completo", MB_OK | MB_ICONINFORMATION);
			}
		}break;
		case IDC_BUTTON1: {
			EnableWindow(GetDlgItem(hwndthis, IDOK), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT1), true);
		}break;
		case IDOK: {
				int setlist = SendDlgItemMessage(hwndthis, listboxsal, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwndthis, listboxsal, LB_GETTEXT, setlist, (LPARAM)Text);
				auxsal = iniciosal;
				while (auxsal != NULL && strcmp(auxsal->idsal, Text) != 0) {
					auxsal = auxsal->sig;
				}
				if (strcmp(auxsal->idsal, Text) == 0) {

					auxsal->asientostot = GetDlgItemInt(hwndthis, IDC_EDIT1, nullptr, false);
					auxfunc = iniciofunc;
					/*while (auxfunc != nullptr) {
						if (strcmp(auxsal->idsal, auxfunc->salas)) {
							auxfunc->asientosdisp = auxsal->asientostot;
						}
					}*/
				}
				SendMessage(GetDlgItem(hwndthis, listboxsal), LB_RESETCONTENT, 0, 0);

				auxsal = iniciosal;
				SendDlgItemMessage(hwndthis, listboxsal, LB_SETCURSEL, 0, 0);
				while (auxsal != nullptr) {
					SendMessage(GetDlgItem(hwndthis, listboxsal), LB_INSERTSTRING, 0, (LPARAM)auxsal->idsal);
					auxsal = auxsal->sig;
				}
				escribir();
				MessageBox(hwndthis, "Sala modificada", "Procedimiendo completo", MB_OK | MB_ICONINFORMATION);
			
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT1), false);
			EnableWindow(GetDlgItem(hwndthis, IDOK), false);
		}break;
		case listboxsal: {
			if (HIWORD(wParam) == LBN_SELCHANGE) {

				HWND hListBox = GetDlgItem(hwndthis, listboxsal);
				int selectedIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
				SendMessage(hListBox, LB_GETTEXT, selectedIndex, (LPARAM)Text);

				auxsal = iniciosal;
				while (auxsal != NULL && strcmp(auxsal->idsal, Text) != 0) {
					auxsal = auxsal->sig;
				}

				if (auxsal != NULL) {
					SetDlgItemText(hwndthis, IDC_EDIT1, auxsal->idsal);
					SetDlgItemInt(hwndthis, IDC_EDIT1, auxsal->asientostot, NULL);
				}
			}
		}break;
		}
	}break;
}
	return 0;
}
LRESULT CALLBACK ProcListEmp(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam) {
	char Text[60] = { 0 };
	switch (msg) {
	case WM_INITDIALOG: {
		HBITMAP foto = (HBITMAP)LoadImage(NULL, datos->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		SendDlgItemMessage(hwndthis, foto1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);


		if (datos != nullptr) {
			SetDlgItemText(hwndthis, nombre1, datos->nombre);
		}

		auxemp = inicioemp;
		SendDlgItemMessage(hwndthis, listboxemp, LB_SETCURSEL, 0, 0);
		while (auxemp != nullptr) {
			SendMessage(GetDlgItem(hwndthis, listboxemp), LB_INSERTSTRING, 0, (LPARAM)auxemp->clave);
			auxemp = auxemp->sig;
		}
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_VER_EMPLEADOS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(listemp), hwndthis, ProcListEmp);
		}break;
		case ID_REGISTRAR_SALAS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(regsal), hwndthis, ProcRegSala);
		}break;
		case ID_VER_SALAS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(listsal), hwndthis, ProcListSala);
		}break;
		case ID_REGISTRAR_FUNCIONES: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(regfunc), hwndthis, ProcRegFunc);
		}break;
		case ID_REGISTRAR_EMPLEADOS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(ventregemp), hwndthis, ProcRegEmp);
		}break;
		case ID_VER_FUNCIONES: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(listfunc), hwndthis, ProcListFunc);
		}break;
		case ID_VENTAS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(ventvent), hwndthis, ProcVentas);
		}break;
		case ID_REPORTEDEVENTAS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(repvent), hwndthis, ProcRepVentas);
		}break;

		case IDC_BUTTON2:
		{
			char elim[100] = { 0 };
			int setlist = SendDlgItemMessage(hwndthis, listboxemp, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwndthis, listboxemp, LB_GETTEXT, setlist, (LPARAM)elim);

			auxemp = inicioemp;
			while (auxemp != NULL && strcmp(auxemp->clave, elim) != 0) {
				auxemp = auxemp->sig;
			}
			if (auxemp != NULL) {

				elimEmp();
				SendMessage(GetDlgItem(hwndthis, listboxemp), LB_RESETCONTENT, 0, 0);

				auxemp = inicioemp;
				SendDlgItemMessage(hwndthis, listboxemp, LB_SETCURSEL, 0, 0);
				while (auxemp != nullptr) {
					SendMessage(GetDlgItem(hwndthis, listboxemp), LB_INSERTSTRING, 0, (LPARAM)auxemp->clave);
					auxemp = auxemp->sig;
				}

				SetDlgItemText(hwndthis, IDC_EDIT1, "");
				SetDlgItemText(hwndthis, IDC_EDIT2, "");
				SetDlgItemText(hwndthis, IDC_EDIT3, "");
				SetDlgItemText(hwndthis, IDC_EDIT4, "");

				escribir();
				MessageBox(hwndthis, "Usuario eliminado", "Procedimiento completo", MB_OK | MB_ICONINFORMATION);
			}
		}break;
		case IDC_BUTTON1: {
			EnableWindow(GetDlgItem(hwndthis, IDC_BUTTON3), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_BUTTON4), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT1), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT2), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT3), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT4), true);
		}break;
		case IDC_BUTTON3: {
			bool flag = validarrfc(hwndthis, IDC_EDIT2);
			bool flag2 = validarletras(hwndthis, IDC_EDIT1);
			if (flag and flag2) {
				int setlist = SendDlgItemMessage(hwndthis, listboxemp, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwndthis, listboxemp, LB_GETTEXT, setlist, (LPARAM)Text);
				auxemp = inicioemp;
				while (auxemp != NULL && strcmp(auxemp->clave, Text) != 0) {
					auxemp = auxemp->sig;
				}
				if (strcmp(auxemp->clave, Text) == 0) {

					GetDlgItemText(hwndthis, IDC_EDIT1, auxemp->nombre, sizeof(auxemp->nombre));
					GetDlgItemText(hwndthis, IDC_EDIT2, auxemp->rfc, sizeof(auxemp->rfc));
					GetDlgItemText(hwndthis, IDC_EDIT3, auxemp->clave, sizeof(auxemp->clave));
					GetDlgItemText(hwndthis, IDC_EDIT4, auxemp->contrasenia, sizeof(auxemp->contrasenia));
					if (zFile[0] != '\0')
					{
						strcpy_s(auxemp->foto, zFile);
					}

				}
				SendMessage(GetDlgItem(hwndthis, listboxemp), LB_RESETCONTENT, 0, 0);

				auxemp = inicioemp;
				SendDlgItemMessage(hwndthis, listboxemp, LB_SETCURSEL, 0, 0);
				while (auxemp != nullptr) {
					SendMessage(GetDlgItem(hwndthis, listboxemp), LB_INSERTSTRING, 0, (LPARAM)auxemp->clave);
					auxemp = auxemp->sig;
				}
				escribir();
				MessageBox(hwndthis, "Empleado modificado", "Procedimiendo completo", MB_OK | MB_ICONINFORMATION);
			}
			else
			{
				MessageBox(hwndthis, "Error al modificar", "Procedimiendo incompleto", MB_OK | MB_ICONWARNING);
			}
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT1), false);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT2), false);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT3), false);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT4), false);
			EnableWindow(GetDlgItem(hwndthis, IDC_BUTTON3), false);
			EnableWindow(GetDlgItem(hwndthis, IDC_BUTTON4), false);
		}break;
		case IDC_BUTTON4: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwndthis;
			ofn.lpstrFilter = "Bitmaps (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 1;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP fotoai;
				fotoai = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
				if (fotoai != NULL) {
					SendDlgItemMessage(hwndthis, bitmap, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fotoai);
				}
				else {
					MessageBox(hwndthis, "Error al cargar la imagen", "Error", MB_OK | MB_ICONERROR);
				}
			}
			else {
				MessageBox(hwndthis, "Seguro que desea cancelar la seleccion del archivo?", "Aviso", MB_OKCANCEL | MB_ICONQUESTION);

			}
		}
		}
		switch (LOWORD(wParam)) {
		case listboxemp: {
			if (HIWORD(wParam) == LBN_SELCHANGE) {

				HWND hListBox = GetDlgItem(hwndthis, listboxemp);
				int selectedIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
				SendMessage(hListBox, LB_GETTEXT, selectedIndex, (LPARAM)Text);

				auxemp = inicioemp;
				while (auxemp != NULL && strcmp(auxemp->clave, Text) != 0) {
					auxemp = auxemp->sig;
				}

				if (auxemp != NULL) {
					SetDlgItemText(hwndthis, IDC_EDIT1, auxemp->nombre);
					SetDlgItemText(hwndthis, IDC_EDIT2, auxemp->rfc);
					SetDlgItemText(hwndthis, IDC_EDIT3, auxemp->clave);
					SetDlgItemText(hwndthis, IDC_EDIT4, auxemp->contrasenia);

					imagen = (HBITMAP)LoadImage(NULL, auxemp->foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					SendDlgItemMessage(hwndthis, bitmap, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);
				}
			}
		}break;
		}
	}break;
	}
	return 0;
}
LRESULT CALLBACK ProcListFunc(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam) {
	char Text[60] = { 0 };
	switch (msg) {
	case WM_INITDIALOG: {
		HBITMAP foto = (HBITMAP)LoadImage(NULL, datos->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		SendDlgItemMessage(hwndthis, foto5, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);


		if (datos != nullptr) {
			SetDlgItemText(hwndthis, nombre5, datos->nombre);
		}

		auxfunc = iniciofunc;
		SendDlgItemMessage(hwndthis, listboxfunc, LB_SETCURSEL, 0, 0);
		while (auxfunc != nullptr) {
			SendMessage(GetDlgItem(hwndthis, listboxfunc), LB_INSERTSTRING, 0, (LPARAM)auxfunc->idfunc);
			auxfunc = auxfunc->sig;
		}
		int	ind = 0;
		auxsal = iniciosal;
		while (auxsal != NULL) {
			SendDlgItemMessage(hwndthis, IDC_COMBO2, CB_INSERTSTRING, (WPARAM)ind, (LPARAM)auxsal->idsal);
			auxsal = auxsal->sig;
			ind++;
		}
		char hora[12][6] = { "9:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00", "17:00", "18:00", "19:00", "20:00" };
		for (int i = 0; i < 12; i++) {
			SendDlgItemMessage(hwndthis, IDC_COMBO1, CB_INSERTSTRING, (WPARAM)i, (LPARAM)hora[i]);
		}
		char idi[2][10] = { "original", "doblado" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwndthis, IDC_COMBO4, CB_INSERTSTRING, (WPARAM)i, (LPARAM)idi[i]);
		}
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
			case ID_VER_EMPLEADOS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listemp), hwndthis, ProcListEmp);
			}break;
			case ID_REGISTRAR_SALAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(regsal), hwndthis, ProcRegSala);
			}break;
			case ID_VER_SALAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listsal), hwndthis, ProcListSala);
			}break;
			case ID_REGISTRAR_FUNCIONES: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(regfunc), hwndthis, ProcRegFunc);
			}break;
			case ID_REGISTRAR_EMPLEADOS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(ventregemp), hwndthis, ProcRegEmp);
			}break;
			case ID_VER_FUNCIONES: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listfunc), hwndthis, ProcListFunc);
			}break;
			case ID_VENTAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(ventvent), hwndthis, ProcVentas);
			}break;
			case ID_REPORTEDEVENTAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(repvent), hwndthis, ProcRepVentas);
			}break;
		case listboxfunc: {
			if (HIWORD(wParam) == LBN_SELCHANGE) {

				HWND hListBox = GetDlgItem(hwndthis, listboxfunc);
				int selectedIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
				SendMessage(hListBox, LB_GETTEXT, selectedIndex, (LPARAM)Text);

				auxfunc = iniciofunc;
				while (auxfunc != NULL && strcmp(auxfunc->idfunc, Text) != 0) {
					auxfunc = auxfunc->sig;
				}

				if (auxfunc != NULL) {
					SetDlgItemText(hwndthis, IDC_EDIT4, auxfunc->nombrepel);
					SetDlgItemInt(hwndthis, IDC_EDIT2, auxfunc->duracion, NULL);
					SetDlgItemText(hwndthis, IDC_EDIT3, auxfunc->clasif);
					SYSTEMTIME  setDate = { 0 };
					ZeroMemory(&setDate, sizeof(setDate));
					setDate.wDay = auxfunc->fechafunc.dia;
					setDate.wMonth = auxfunc->fechafunc.mes;
					setDate.wYear = auxfunc->fechafunc.anio;
					SendDlgItemMessage(hwndthis, IDC_DATETIMEPICKER1, DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&setDate);
					SetDlgItemText(hwndthis, IDC_COMBO1, auxfunc->hora);
					SetDlgItemText(hwndthis, IDC_COMBO4, auxfunc->idioma);
					SetDlgItemInt(hwndthis, IDC_EDIT9, auxfunc->preciobol, NULL);
					auxsal = iniciosal;
					while (auxsal != NULL) {
						if (strcmp(auxfunc->salas, auxsal->idsal) == 0) {
							SetDlgItemText(hwndthis, IDC_COMBO2, auxfunc->salas);
							SetDlgItemInt(hwndthis, IDC_EDIT6, auxsal->asientostot, 0);
						}
						auxsal = auxsal->sig;
					}
					imagen = (HBITMAP)LoadImage(NULL, auxfunc->poster, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
					SendDlgItemMessage(hwndthis, bitmap111, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);
				}
			}
		}break;
		case IDCANCEL:
		{
			char elim[100] = { 0 };
			int setlist = SendDlgItemMessage(hwndthis, listboxfunc, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwndthis, listboxfunc, LB_GETTEXT, setlist, (LPARAM)elim);

			auxfunc = iniciofunc;
			while (auxfunc != NULL && strcmp(auxfunc->idfunc, elim) != 0) {
				auxfunc = auxfunc->sig;
			}
			if (auxfunc != NULL) {

				elimFunc();
				SendMessage(GetDlgItem(hwndthis, listboxfunc), LB_RESETCONTENT, 0, 0);

				auxfunc = iniciofunc;
				SendDlgItemMessage(hwndthis, listboxfunc, LB_SETCURSEL, 0, 0);
				while (auxfunc != nullptr) {
					SendMessage(GetDlgItem(hwndthis, listboxfunc), LB_INSERTSTRING, 0, (LPARAM)auxfunc->idfunc);
					auxfunc = auxfunc->sig;
				}

				SetDlgItemText(hwndthis, IDC_EDIT4, "");
				SetDlgItemText(hwndthis, IDC_EDIT2, "");
				SetDlgItemText(hwndthis, IDC_EDIT3, "");
				SetDlgItemText(hwndthis, IDC_COMBO2, "");
				SetDlgItemText(hwndthis, IDC_COMBO4, "");
				SetDlgItemText(hwndthis, IDC_EDIT6, "");
				SetDlgItemText(hwndthis, IDC_COMBO1, "");
				SetDlgItemText(hwndthis, IDC_EDIT9, "");

				escribir();
				MessageBox(hwndthis, "Funcion eliminada", "Procedimiento completo", MB_OK | MB_ICONINFORMATION);
			}
		}break;
		case IDC_BUTTON2: {
			EnableWindow(GetDlgItem(hwndthis, IDOK), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_BUTTON4), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT4), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT2), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT3), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT4), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_COMBO2), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_COMBO4), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_COMBO1), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_DATETIMEPICKER1), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT9), true);
			EnableWindow(GetDlgItem(hwndthis, IDC_BUTTON3), true);
		}break;
		case IDOK: {
				int setlist = SendDlgItemMessage(hwndthis, listboxfunc, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwndthis, listboxfunc, LB_GETTEXT, setlist, (LPARAM)Text);
				auxfunc = iniciofunc;
				while (auxfunc != NULL && strcmp(auxfunc->idfunc, Text) != 0) {
					auxfunc = auxfunc->sig;
				}
				if (strcmp(auxfunc->idfunc, Text) == 0) {

					GetDlgItemText(hwndthis, IDC_EDIT4, auxfunc->nombrepel, sizeof(auxfunc->nombrepel));
					auxfunc->duracion = GetDlgItemInt(hwndthis, IDC_EDIT2, nullptr, false);
					GetDlgItemText(hwndthis, IDC_EDIT3, auxfunc->clasif, sizeof(auxfunc->clasif));
					SendDlgItemMessage(hwndthis, IDC_COMBO2, WM_GETTEXT, 128, (LPARAM)auxfunc->salas);
					SendDlgItemMessage(hwndthis, IDC_COMBO4, WM_GETTEXT, 128, (LPARAM)auxfunc->idioma);
					SendDlgItemMessage(hwndthis, IDC_COMBO1, WM_GETTEXT, 128, (LPARAM)auxfunc->hora);
					auxfunc->preciobol = GetDlgItemInt(hwndthis, IDC_EDIT9, nullptr, false);
					SYSTEMTIME fechaSeleccionada = { 0 };
					ZeroMemory(&fechaSeleccionada, sizeof(fechaSeleccionada));
					SendDlgItemMessage(hwndthis, IDC_DATETIMEPICKER1, DTM_GETSYSTEMTIME, 0, (LPARAM)&fechaSeleccionada);
					auxfunc->fechafunc.dia = fechaSeleccionada.wDay;
					auxfunc->fechafunc.mes = fechaSeleccionada.wMonth;
					auxfunc->fechafunc.anio = fechaSeleccionada.wYear;
					if (zFile[0] != '\0')
					{
						strcpy_s(auxfunc->poster, zFile);
					}

				}
				SendMessage(GetDlgItem(hwndthis, listboxfunc), LB_RESETCONTENT, 0, 0);

				auxfunc = iniciofunc;
				SendDlgItemMessage(hwndthis, listboxfunc, LB_SETCURSEL, 0, 0);
				while (auxfunc != nullptr) {
					SendMessage(GetDlgItem(hwndthis, listboxfunc), LB_INSERTSTRING, 0, (LPARAM)auxfunc->idfunc);
					auxfunc = auxfunc->sig;
				}
				escribir();
				MessageBox(hwndthis, "Funcion modificada", "Procedimiendo completo", MB_OK | MB_ICONINFORMATION);
	
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT4), false);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT2), false);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT3), false);
			EnableWindow(GetDlgItem(hwndthis, IDC_COMBO2), false);
			EnableWindow(GetDlgItem(hwndthis, IDC_COMBO4), false);
			EnableWindow(GetDlgItem(hwndthis, IDC_DATETIMEPICKER1), false);
			EnableWindow(GetDlgItem(hwndthis, IDC_COMBO1), false);
			EnableWindow(GetDlgItem(hwndthis, IDC_EDIT9), false);
		}break;
		case IDC_BUTTON3: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwndthis;
			ofn.lpstrFilter = "Bitmaps (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 1;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP fotoai;
				fotoai = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
				if (fotoai != NULL) {
					SendDlgItemMessage(hwndthis, bitmap111, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fotoai);
				}
				else {
					MessageBox(hwndthis, "Error al cargar la imagen", "Error", MB_OK | MB_ICONERROR);
				}
			}
			else {
				MessageBox(hwndthis, "Seguro que desea cancelar la seleccion del archivo?", "Aviso", MB_OKCANCEL | MB_ICONQUESTION);

			}
		}
	}
	}break;
	}
	return 0;
}
LRESULT CALLBACK ProcRepVentas(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		HBITMAP foto = (HBITMAP)LoadImage(NULL, datos->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		SendDlgItemMessage(hwndthis, foto8, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);


		if (datos != nullptr) {
			SetDlgItemText(hwndthis, nombre8, datos->nombre);
		}
	}break;
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
			case ID_VER_EMPLEADOS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listemp), hwndthis, ProcListEmp);
			}break;
			case ID_REGISTRAR_SALAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(regsal), hwndthis, ProcRegSala);
			}break;
			case ID_VER_SALAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listsal), hwndthis, ProcListSala);
			}break;
			case ID_REGISTRAR_FUNCIONES: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(regfunc), hwndthis, ProcRegFunc);
			}break;
			case ID_REGISTRAR_EMPLEADOS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(ventregemp), hwndthis, ProcRegEmp);
			}break;
			case ID_VER_FUNCIONES: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(listfunc), hwndthis, ProcListFunc);
			}break;
			case ID_VENTAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(ventvent), hwndthis, ProcVentas);
			}break;
			case ID_REPORTEDEVENTAS: {
				EndDialog(hwndthis, 0);
				DialogBox(global, MAKEINTRESOURCE(repvent), hwndthis, ProcRepVentas);
			}break;
			case IDOK:
			{
				SYSTEMTIME date = { 0 };

				SendDlgItemMessage(hwndthis, IDC_DATETIMEPICKER1, DTM_GETSYSTEMTIME, 0, (LPARAM)&date);

				int tarjetatot = 0;
				int efectivotot = 0;
				int totaltotal = 0;

				auxvent = iniciovent;
				while (auxvent != NULL) {


					if (auxvent->fechaventa.dia == date.wDay and auxvent->fechaventa.mes == date.wMonth and auxvent->fechaventa.anio == date.wYear)
					{

						if (strcmp(auxvent->tipodepago, "tarjeta") == 0)
						{
							tarjetatot = tarjetatot + auxvent->venta;
						}
						else if (strcmp(auxvent->tipodepago, "efectivo") == 0)
						{
							efectivotot = efectivotot + auxvent->venta;
						}

					}
					auxvent = auxvent->sig;
				}
				totaltotal = tarjetatot + efectivotot;
				SetDlgItemInt(hwndthis, IDC_EDIT2, tarjetatot, NULL);
				SetDlgItemInt(hwndthis, IDC_EDIT1, efectivotot, NULL);
				SetDlgItemInt(hwndthis, IDC_EDIT3, totaltotal, NULL);


				MessageBox(hwndthis, "Reporte creado", "Aviso", MB_OK | MB_ICONQUESTION);
			}break;
			}

		} break;
	}
	return 0;
}
LRESULT CALLBACK ProcVentas(HWND hwndthis, UINT msg, WPARAM wParam, LPARAM lParam) {
	char Text[60] = { 0 };
	switch (msg) {
	case WM_INITDIALOG: {
		HBITMAP foto = (HBITMAP)LoadImage(NULL, datos->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		SendDlgItemMessage(hwndthis, foto7, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);

		if (datos != nullptr) {
			SetDlgItemText(hwndthis, nombre7, datos->nombre);
		}

		char tipo[2][11] = { "efectivo", "tarjeta"};
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwndthis, IDC_COMBO3, CB_INSERTSTRING, (WPARAM)i, (LPARAM)tipo[i]);
		}

		auxfunc = iniciofunc;
		SendDlgItemMessage(hwndthis, IDC_LIST1, LB_SETCURSEL, 0, 0);
		while (auxfunc != nullptr) {
			SendMessage(GetDlgItem(hwndthis, IDC_LIST1), LB_INSERTSTRING, 0, (LPARAM)auxfunc->idfunc);
			auxfunc = auxfunc->sig;
		}
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_VER_EMPLEADOS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(listemp), hwndthis, ProcListEmp);
		}break;
		case ID_REGISTRAR_SALAS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(regsal), hwndthis, ProcRegSala);
		}break;
		case ID_VER_SALAS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(listsal), hwndthis, ProcListSala);
		}break;
		case ID_REGISTRAR_FUNCIONES: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(regfunc), hwndthis, ProcRegFunc);
		}break;
		case ID_REGISTRAR_EMPLEADOS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(ventregemp), hwndthis, ProcRegEmp);
		}break;
		case ID_VER_FUNCIONES: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(listfunc), hwndthis, ProcListFunc);
		}break;
		case ID_VENTAS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(ventvent), hwndthis, ProcVentas);
		}break;
		case ID_REPORTEDEVENTAS: {
			EndDialog(hwndthis, 0);
			DialogBox(global, MAKEINTRESOURCE(repvent), hwndthis, ProcRepVentas);
		}break;

		case IDC_EDIT1:
		{
			if (HIWORD(wParam) == EN_CHANGE)
			{
				int total;
				int num1 = GetDlgItemInt(hwndthis, IDC_EDIT1, NULL, true);
				int num2 = GetDlgItemInt(hwndthis, IDC_EDIT9, NULL, true);
				total = num1 * num2;
				SetDlgItemInt(hwndthis, IDC_EDIT3, total, NULL);
			}
		}break;

		case IDOK: {
			bool error = false;
			auxvent = new ventas;
			venta();
			SendDlgItemMessage(hwndthis, IDC_COMBO3, WM_GETTEXT, 128, (LPARAM)auxvent->tipodepago);
			int setlist = SendDlgItemMessage(hwndthis, IDC_LIST1, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwndthis, IDC_LIST1, LB_GETTEXT, setlist, (LPARAM)Text);
			auxfunc = iniciofunc;
			while (auxfunc != NULL && strcmp(auxfunc->idfunc, Text) != 0) {
				auxfunc = auxfunc->sig;
			}
			if (auxfunc != NULL) {
				int resta = GetDlgItemInt(hwndthis, IDC_EDIT1, NULL, true);
				if (resta < 0)
				{
					MessageBox(hwndthis, "No se puede vender un numero negativo de boletos", "Procedimiendo incompleto", MB_OK | MB_ICONINFORMATION);
					error = true;
				}
				if (auxfunc->asientosdisp >= resta) {
					auxfunc->asientosdisp = auxfunc->asientosdisp - resta;
					SetDlgItemInt(hwndthis, IDC_EDIT7, auxfunc->asientosdisp, NULL);
				}
				else
				{
					MessageBox(hwndthis, "No hay suficientes asientos disponibles", "Procedimiendo incompleto", MB_OK | MB_ICONINFORMATION);
					error = true;
				}
				SYSTEMTIME fechaSeleccionada = { 0 };

				ZeroMemory(&fechaSeleccionada, sizeof(fechaSeleccionada));

				SendDlgItemMessage(hwndthis, IDC_DATETIMEPICKER1, DTM_GETSYSTEMTIME, 0, (LPARAM)&fechaSeleccionada);

				auxvent->fechaventa.dia = fechaSeleccionada.wDay;
				auxvent->fechaventa.mes = fechaSeleccionada.wMonth;
				auxvent->fechaventa.anio = fechaSeleccionada.wYear;
				
				auxvent->venta = GetDlgItemInt(hwndthis, IDC_EDIT3, NULL, true);

				if (error == false){
					escribir();
				MessageBox(hwndthis, "Venta registrada", "Procedimiento completo", MB_OK | MB_ICONINFORMATION);
				}
			}
		}
		case IDC_LIST1: {
			if (HIWORD(wParam) == LBN_SELCHANGE) {

				HWND hListBox = GetDlgItem(hwndthis, IDC_LIST1);
				int selectedIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
				SendMessage(hListBox, LB_GETTEXT, selectedIndex, (LPARAM)Text);

				auxfunc = iniciofunc;
				while (auxfunc != NULL && strcmp(auxfunc->idfunc, Text) != 0) {
					auxfunc = auxfunc->sig;
				}

				if (auxfunc != NULL) {
					SetDlgItemText(hwndthis, nompel, auxfunc->nombrepel);
					SYSTEMTIME  setDate = { 0 };
					ZeroMemory(&setDate, sizeof(setDate));
					setDate.wDay = auxfunc->fechafunc.dia;
					setDate.wMonth = auxfunc->fechafunc.mes;
					setDate.wYear = auxfunc->fechafunc.anio;
					SendDlgItemMessage(hwndthis, IDC_DATETIMEPICKER1, DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&setDate);
					SetDlgItemText(hwndthis, IDC_COMBO4, auxfunc->hora);
					SetDlgItemText(hwndthis, IDC_COMBO5, auxfunc->idioma);
					SetDlgItemInt(hwndthis, IDC_EDIT9, auxfunc->preciobol, NULL);
					auxsal = iniciosal;
					while (auxsal != NULL) {
						if (strcmp(auxfunc->salas, auxsal->idsal) == 0) {
							SetDlgItemText(hwndthis, IDC_COMBO2, auxfunc->salas);
							SetDlgItemInt(hwndthis, IDC_EDIT6, auxsal->asientostot, 0);
						}
						auxsal = auxsal->sig;
					}
					SetDlgItemInt(hwndthis, IDC_EDIT7, auxfunc->asientosdisp, NULL);

					imagen = (HBITMAP)LoadImage(NULL, auxfunc->poster, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					SendDlgItemMessage(hwndthis, bitmap0, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);
				}
			}
		}break;
		}
	}break;
	}
	return 0;
}

void regEmp()
{
	if (inicioemp == nullptr)
	{
		inicioemp = auxemp;
		finemp = inicioemp;
		finemp->ant = nullptr;
		finemp->sig = nullptr;
	}
	else
	{

		finemp->sig = auxemp;
		auxemp->ant = finemp;
		auxemp->sig = nullptr;
	}
	finemp = auxemp;

}
void regSala()
{
	if (iniciosal == nullptr)
	{
		iniciosal = auxsal;
		finsal = iniciosal;
		finsal->ant = nullptr;
		finsal->sig = nullptr;
	}
	else
	{

		finsal->sig = auxsal;
		auxsal->ant = finsal;
		auxsal->sig = nullptr;
	}
	finsal = auxsal;

}
void regFunc()
{
	if (iniciofunc == nullptr)
	{
		iniciofunc = auxfunc;
		finfunc = iniciofunc;
		finfunc->ant = nullptr;
		finfunc->sig = nullptr;
	}
	else
	{
		finfunc->sig = auxfunc;
		auxfunc->ant = finfunc;
		auxfunc->sig = nullptr;
	}
	finfunc = auxfunc;

}
void elimEmp() {
		if (auxemp == inicioemp) {
			inicioemp = auxemp->sig;
			if (inicioemp != nullptr) {
				inicioemp->ant = nullptr;
			}
		}
		else if (auxemp == finemp) {
			finemp = auxemp->ant;
			finemp->sig = nullptr;
		}
		else {
			auxemp->sig->ant = auxemp->ant;
			auxemp->ant->sig = auxemp->sig;
		}
		delete auxemp;
		auxemp = nullptr;
	}
void elimSala() {
		if (auxsal == iniciosal) {
			iniciosal = auxsal->sig;
			if (iniciosal != nullptr) {
				iniciosal->ant = nullptr;
			}
		}
		else if (auxsal == finsal) {
			finsal = auxsal->ant;
			finsal->sig = nullptr;
		}
		else {
			auxsal->sig->ant = auxsal->ant;
			auxsal->ant->sig = auxsal->sig;
		}
		delete auxsal;
		auxsal = nullptr;
	}
void elimFunc() {
		if (auxfunc == iniciofunc) {
			iniciofunc = auxfunc->sig;
			if (iniciofunc != nullptr) {
				iniciofunc->ant = nullptr;
			}
		}
		else if (auxfunc == finfunc) {
			finfunc = auxfunc->ant;
			finfunc->sig = nullptr;
		}
		else {
			auxfunc->sig->ant = auxfunc->ant;
			auxfunc->ant->sig = auxfunc->sig;
		}
		delete auxfunc;
		auxfunc = nullptr;
	}
void venta() 
{
	if (iniciovent == nullptr)
	{
		iniciovent = auxvent;
		finvent = iniciovent;
		finvent->ant = nullptr;
		finvent->sig = nullptr;
	}
	else
	{

		finvent->sig = auxvent;
		auxvent->ant = finvent;
		auxvent->sig = nullptr;
	}
	finvent = auxvent;

}

void escribir()
{
	auxemp = inicioemp;
	ofstream archivoemp;
	archivoemp.open("D:\\Documentos\\C++\\metnix10\\metnix10\\Empleados.dat", ios::out | ios::trunc | ios::binary);
	if (archivoemp.is_open()) {
		while (auxemp != nullptr) {
			archivoemp.write((char*)auxemp, sizeof(*auxemp));
			auxemp = auxemp->sig;
		}
	}
	auxsal = iniciosal;
	ofstream archivosal;
	archivosal.open("D:\\Documentos\\C++\\metnix10\\metnix10\\Salas.dat", ios::out | ios::trunc | ios::binary);
	if (archivosal.is_open()) {
		while (auxsal != nullptr) {
			archivosal.write((char*)auxsal, sizeof(*auxsal));
			auxsal = auxsal->sig;
		}
	}
	auxfunc = iniciofunc;
	ofstream archivofunc;
	archivofunc.open("D:\\Documentos\\C++\\metnix10\\metnix10\\Funciones.dat", ios::out | ios::trunc | ios::binary);
	if (archivofunc.is_open()) {
		while (auxfunc != nullptr) {
			archivofunc.write((char*)auxfunc, sizeof(*auxfunc));
			auxfunc = auxfunc->sig;
		}
	}
	auxvent = iniciovent;
	ofstream archivovent;
	archivovent.open("D:\\Documentos\\C++\\metnix10\\metnix10\\Ventas.dat", ios::out | ios::trunc | ios::binary);
	if (archivovent.is_open()) {
		while (auxvent != nullptr) {
			archivovent.write((char*)auxvent, sizeof(*auxvent));
			auxvent = auxvent->sig;
		}
	}
}
void leer()
{
	ifstream archivoemp;
	archivoemp.open("D:\\Documentos\\C++\\metnix10\\metnix10\\Empleados.dat", ios::in | ios::binary);

	if (archivoemp.is_open()) {

		auxemp = new empleados;
		while (!archivoemp.read((char*)auxemp,
			sizeof(empleados)).eof()) {

			if (inicioemp == nullptr) {
				inicioemp = auxemp;
				inicioemp->sig = nullptr;
				inicioemp->ant = nullptr;
				finemp = auxemp;
			}
			else {
				finemp->sig = auxemp;
				auxemp->ant = finemp;
				finemp = auxemp;
				finemp->sig = nullptr;
			}
			auxemp = new empleados;
		}
		archivoemp.close();
		delete auxemp;
	}
	ifstream archivosal;
	archivosal.open("D:\\Documentos\\C++\\metnix10\\metnix10\\Salas.dat", ios::in | ios::binary);

	if (archivosal.is_open()) {

		auxsal = new salas;
		while (!archivosal.read((char*)auxsal,
			sizeof(salas)).eof()) {

			if (iniciosal == nullptr) {
				iniciosal = auxsal;
				iniciosal->sig = nullptr;
				iniciosal->ant = nullptr;
				finsal = auxsal;
			}
			else {
				finsal->sig = auxsal;
				auxsal->ant = finsal;
				finsal = auxsal;
				finsal->sig = nullptr;
			}
			auxsal = new salas;
		}
		archivosal.close();
		delete auxsal;
	}

	ifstream archivoFunc;
	archivoFunc.open("D:\\Documentos\\C++\\metnix10\\metnix10\\Funciones.dat", ios::in | ios::binary);

	if (archivoFunc.is_open()) {

		auxfunc = new funciones;
		while (!archivoFunc.read((char*)auxfunc,
			sizeof(funciones)).eof()) {

			if (iniciofunc == nullptr) {
				iniciofunc = auxfunc;
				iniciofunc->sig = nullptr;
				iniciofunc->ant = nullptr;
				finfunc = auxfunc;
			}
			else {
				finfunc->sig = auxfunc;
				auxfunc->ant = finfunc;
				finfunc = auxfunc;
				finfunc->sig = nullptr;
			}
			auxfunc = new funciones;
		}
		archivoFunc.close();
		delete auxfunc;
	}
	ifstream archivovent;
	archivovent.open("D:\\Documentos\\C++\\metnix10\\metnix10\\Ventas.dat", ios::in | ios::binary);

	if (archivovent.is_open()) {

		auxvent = new ventas;
		while (!archivovent.read((char*)auxvent,
			sizeof(ventas)).eof()) {

			if (iniciovent == nullptr) {
				iniciovent = auxvent;
				iniciovent->sig = nullptr;
				iniciovent->ant = nullptr;
				finvent = auxvent;
			}
			else {
				finvent->sig = auxvent;
				auxvent->ant = finvent;
				finvent = auxvent;
				finvent->sig = nullptr;
			}
			auxvent = new ventas;
		}
		archivovent.close();
		delete auxvent;
	}
}

bool validarrfc(HWND hwndthis, int IDEDIT)
{
	char text[11];
	GetDlgItemText(hwndthis, IDEDIT, text, sizeof(text));

	bool flag = true;

	if (strlen(text) != 10) {
		flag = false;
		MessageBox(hwndthis, "RFC incorrecto . Debe ser de 10 caracteres", "Error de validación", MB_OK | MB_ICONERROR);
	}
	else {
		for (int i = 0; i < 4; ++i) {
			if (!isalpha(text[i])) {
				flag = false;
				break;
			}
		}
		for (int i = 4; i < 10; ++i) {
			if (!isdigit(text[i])) {
				flag = false;
				break;
			}
		}
	}
	if (flag) {

		char daytext[3] = { text[4], text[5], '\0' };
		int day = atoi(daytext);
		if (day < 1,  day > 31) {
			flag = false;
		}

		char monthtext[3] = { text[6], text[7], '\0' };
		int month = atoi(monthtext);
		if (month < 1,  month > 12) {
			flag = false;
		}
	}
	if (flag) {
		MessageBox(hwndthis, "RFC correcto", "Validación", MB_OK | MB_ICONINFORMATION);
	}
	else {
		MessageBox(hwndthis, "RFC incorrecto . Debe ser 4 letras seguidas de 6 números, y un mes y día válidos", "Error de validación", MB_OK | MB_ICONERROR);
	}
	return flag;
}
bool validarletras(HWND hwndthis, int IDEDIT)
{
	bool flag = true;
	char text[128];
	GetDlgItemText(hwndthis, IDEDIT, text, sizeof(text));

	for (int i = 0; text[i] != '\0'; ++i) {
		if (text[i] != 'ñ' and text[i] != 'á' and text[i] != 'é' and text[i] != 'í' and text[i] != 'ó' and text[i] != 'ú')
		{
			if (!isalpha(text[i]) && !isspace(text[i])) {
				flag = false;

			}
		}
	}
	if (flag == false)
	{
		MessageBox(hwndthis, "Nombre inválido. Solamente acepta letras", "Error de validación", MB_OK | MB_ICONERROR);
	}
	return flag;
}