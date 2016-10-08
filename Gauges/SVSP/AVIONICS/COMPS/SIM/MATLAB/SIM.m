function varargout = SIM(varargin)
% SIM M-file for SIM.fig
%      SIM, by itself, creates a new SIM or raises the existing
%      singleton*.
%
%      H = SIM returns the handle to a new SIM or the handle to
%      the existing singleton*.
%
%      SIM('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in SIM.M with the given input arguments.
%
%      SIM('Property','Value',...) creates a new SIM or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before SIM_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to SIM_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Copyright 2002-2003 The MathWorks, Inc.

% Edit the above text to modify the response to help SIM

% Last Modified by GUIDE v2.5 03-Mar-2010 09:54:35

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @SIM_OpeningFcn, ...
                   'gui_OutputFcn',  @SIM_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before SIM is made visible.
function SIM_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to SIM (see VARARGIN)

% Choose default command line output for SIM
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

global Bx By Bz; 
global Cx Cy Cz;
global Ax Ay Az;
global K;
global SIM_MIN_AIRSPEED;
global SIM_MAX_AIRSPEED;
global SIM_MAX_Bx;
global SIM_MAX_By;
global SIM_MAX_Bz;
global SIM_MAX_Cx;
global SIM_MAX_Cy;
global SIM_MAX_Cz;
global SIM_MIN_AoA;
global SIM_MAX_AoA;


K                = 1.0;
SIM_MIN_AIRSPEED = 0;
SIM_MAX_AIRSPEED = 1000.0;
SIM_MAX_Bx       = 1.0;
SIM_MAX_By       = 1.0;
SIM_MAX_Bz       = 1.0;
SIM_MAX_Cx       = 10.0;
SIM_MAX_Cy       = 10.0;
SIM_MAX_Cz       = 10.0;
Bx               = 0.0200; 
By               = 0.0100; 
Bz               = 0.1500;
Cx               = 2.01; 
Cy               = 1.72; 
Cz               = 3.05;
SIM_MIN_AoA      = 0;
SIM_MAX_AoA      = 2*pi;


set(handles.Bx_slider,'Value', Bx/SIM_MAX_Bx);
set(handles.Bx_edit,'String', num2str(Bx));
set(handles.By_slider,'Value', By/SIM_MAX_By);
set(handles.By_edit,'String', num2str(By));
set(handles.Bz_slider,'Value', Bz/SIM_MAX_Bz);
set(handles.Bz_edit,'String', num2str(Bz));

set(handles.Cx_slider,'Value', Cx/SIM_MAX_Cx);
set(handles.Cx_edit,'String', num2str(Cx));
set(handles.Cy_slider,'Value', Cy/SIM_MAX_Cy);
set(handles.Cy_edit,'String', num2str(Cy));
set(handles.Cz_slider,'Value', Cz/SIM_MAX_Cz);
set(handles.Cz_edit,'String', num2str(Cz));

SIM_DrawAirspeedFactorFunction(handles);
%SIM_DrawAoAFactorFunction(handles);

% UIWAIT makes SIM wait for user response (see UIRESUME)
% uiwait(handles.figure1);



% --- Outputs from this function are returned to the command line.
function varargout = SIM_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Draws the AirspeedFactor plot.
function SIM_DrawAirspeedFactorFunction(handles)
global Bx By Bz;
global Cx Cy Cz;
global Ax Ay Az;
global SIM_MIN_AIRSPEED;
global SIM_MAX_AIRSPEED;
global SIM_MIN_AoA;
global SIM_MAX_AoA;
global K;


X = SIM_MIN_AIRSPEED:5.0:SIM_MAX_AIRSPEED;
B = 0;

%fBVx = K*(Bx*(X - B)./sqrt(1 + Bx^2*(X - B).^2));
fBVx = K*(1 - 1./(1 + Bx*(X - B)));
fBVy = K*(1 - 1./(1 + By*(X - B)));
fBVz = K*(1 - 1./(1 + Bz*(X - B)));

% --- Draws LiftFactor plot.
axes(handles.AirspeedFactorAxis);
plot(X, fBVx, X, fBVy, X, fBVz, 'LineWidth',1.5);
axis([SIM_MIN_AIRSPEED SIM_MAX_AIRSPEED -1.2*K 1.2*K]);
grid on;
legend('Aileron (fBVx)', 'Elevator (fBVy)', 'Rudder (fBVz)', 2, 'Location', 'NorthEast');


AoA = SIM_MIN_AoA:0.0175:SIM_MAX_AoA;

C = pi*50.0/180.0;
fBAx = -K*(Cx*(AoA - C)./sqrt(1 + Cx^2*(AoA - C).^2));
fBAy = -K*(Cy*(AoA - C)./sqrt(1 + Cy^2*(AoA - C).^2));
fBAz = -K*(Cz*(AoA - C)./sqrt(1 + Cz^2*(AoA - C).^2));

% --- Draws AoAFactor plot.
axes(handles.AoAFactorAxis);
plot(AoA, fBAx, AoA, fBAy, AoA, fBAz, 'LineWidth',1.5)
%%plot(Xin,Xout, Xin,Yout, Xin,Zout)
axis([SIM_MIN_AoA SIM_MAX_AoA -1.2*K 1.2*K])
grid on
legend('Aileron (fBAx)', 'Elevator (fBAy)', 'Rudder (fBAz)', 2, 'Location', 'NorthEast')


fTx = fBVx.*fBAx;
fTy = fBVy.*fBAy;
fTy = fBVz.*fBAz;

% --- Draws AoAFactor plot.
axes(handles.TorqueFactorAxis);
plot(AoA, fTx, AoA, fTy, AoA, fTz, 'LineWidth',1.5)
%%plot(Xin,Xout, Xin,Yout, Xin,Zout)
axis([SIM_MIN_AoA SIM_MAX_AoA -1.2*K 1.2*K])
grid on
legend('Aileron (fBAx)', 'Elevator (fBAy)', 'Rudder (fBAz)', 2, 'Location', 'NorthEast')


% --- Executes on slider movement.
function Bx_slider_Callback(hObject, eventdata, handles)
% hObject    handle to Bx_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global SIM_MAX_Bx;
global Bx;

Bx    = SIM_MAX_Bx*get(hObject,'Value');
set(handles.Bx_edit,'String', num2str(Bx));
SIM_DrawAirspeedFactorFunction(handles);

% --- Executes during object creation, after setting all properties.
function Bx_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Bx_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background, change
%       'usewhitebg' to 0 to use default.  See ISPC and COMPUTER.
usewhitebg = 1;
if usewhitebg
    set(hObject,'BackgroundColor',[.9 .9 .9]);
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function Bx_edit_Callback(hObject, eventdata, handles)
% hObject    handle to Bx_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Bx_edit as text
%        str2double(get(hObject,'String')) returns contents of Bx_edit as a double


% --- Executes during object creation, after setting all properties.
function Bx_edit_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Bx_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on slider movement.
function By_slider_Callback(hObject, eventdata, handles)
% hObject    handle to By_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global SIM_MAX_By;
global By;

By    = SIM_MAX_By*get(hObject,'Value');
set(handles.By_edit,'String', num2str(By));
SIM_DrawAirspeedFactorFunction(handles);


% --- Executes during object creation, after setting all properties.
function By_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to By_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background, change
%       'usewhitebg' to 0 to use default.  See ISPC and COMPUTER.
usewhitebg = 1;
if usewhitebg
    set(hObject,'BackgroundColor',[.9 .9 .9]);
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function By_edit_Callback(hObject, eventdata, handles)
% hObject    handle to By_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of By_edit as text
%        str2double(get(hObject,'String')) returns contents of By_edit as a double


% --- Executes during object creation, after setting all properties.
function By_edit_CreateFcn(hObject, eventdata, handles)
% hObject    handle to By_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on slider movement.
function Bz_slider_Callback(hObject, eventdata, handles)
% hObject    handle to Bz_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global SIM_MAX_Bz;
global Bz;

Bz    = SIM_MAX_Bz*get(hObject,'Value');
set(handles.Bz_edit,'String', num2str(Bz));
SIM_DrawAirspeedFactorFunction(handles);


% --- Executes during object creation, after setting all properties.
function Bz_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Bz_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background, change
%       'usewhitebg' to 0 to use default.  See ISPC and COMPUTER.
usewhitebg = 1;
if usewhitebg
    set(hObject,'BackgroundColor',[.9 .9 .9]);
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function Bz_edit_Callback(hObject, eventdata, handles)
% hObject    handle to Bz_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Bz_edit as text
%        str2double(get(hObject,'String')) returns contents of Bz_edit as a double


% --- Executes during object creation, after setting all properties.
function Bz_edit_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Bz_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on slider movement.
function Cx_slider_Callback(hObject, eventdata, handles)
% hObject    handle to Cx_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global SIM_MAX_Cx;
global Cx;

Cx    = SIM_MAX_Cx*get(hObject,'Value');
set(handles.Cx_edit,'String', num2str(Cx));
SIM_DrawAoAFactorFunction(handles);


% --- Executes during object creation, after setting all properties.
function Cx_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Cx_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background, change
%       'usewhitebg' to 0 to use default.  See ISPC and COMPUTER.
usewhitebg = 1;
if usewhitebg
    set(hObject,'BackgroundColor',[.9 .9 .9]);
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function Cx_edit_Callback(hObject, eventdata, handles)
% hObject    handle to Cx_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Cx_edit as text
%        str2double(get(hObject,'String')) returns contents of Cx_edit as a double


% --- Executes during object creation, after setting all properties.
function Cx_edit_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Cx_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on slider movement.
function Cy_slider_Callback(hObject, eventdata, handles)
% hObject    handle to Cy_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global SIM_MAX_Cy;
global Cy;

Cy    = SIM_MAX_Cy*get(hObject,'Value');
set(handles.Cy_edit,'String', num2str(Cy));
SIM_DrawAoAFactorFunction(handles);


% --- Executes during object creation, after setting all properties.
function Cy_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Cy_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background, change
%       'usewhitebg' to 0 to use default.  See ISPC and COMPUTER.
usewhitebg = 1;
if usewhitebg
    set(hObject,'BackgroundColor',[.9 .9 .9]);
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function Cy_edit_Callback(hObject, eventdata, handles)
% hObject    handle to Cy_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Cy_edit as text
%        str2double(get(hObject,'String')) returns contents of Cy_edit as a double


% --- Executes during object creation, after setting all properties.
function Cy_edit_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Cy_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on slider movement.
function Cz_slider_Callback(hObject, eventdata, handles)
% hObject    handle to Cz_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global SIM_MAX_Cz;
global Cz;

Cz    = SIM_MAX_Cz*get(hObject,'Value');
set(handles.Cz_edit,'String', num2str(Cz));
SIM_DrawAoAFactorFunction(handles);


% --- Executes during object creation, after setting all properties.
function Cz_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Cz_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background, change
%       'usewhitebg' to 0 to use default.  See ISPC and COMPUTER.
usewhitebg = 1;
if usewhitebg
    set(hObject,'BackgroundColor',[.9 .9 .9]);
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function Cz_edit_Callback(hObject, eventdata, handles)
% hObject    handle to Cz_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Cz_edit as text
%        str2double(get(hObject,'String')) returns contents of Cz_edit as a double


% --- Executes during object creation, after setting all properties.
function Cz_edit_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Cz_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end







% --- Executes during object creation, after setting all properties.
function figure1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


