function varargout = FCS_GUIDE(varargin)
% FCS_GUIDE M-file for FCS_GUIDE.fig
%      FCS_GUIDE, by itself, creates a new FCS_GUIDE or raises the existing
%      singleton*.
%
%      H = FCS_GUIDE returns the handle to a new FCS_GUIDE or the handle to
%      the existing singleton*.
%
%      FCS_GUIDE('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in FCS_GUIDE.M with the given input arguments.
%
%      FCS_GUIDE('Property','Value',...) creates a new FCS_GUIDE or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before FCS_GUIDE_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to FCS_GUIDE_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Copyright 2002-2003 The MathWorks, Inc.

% Edit the above text to modify the response to help FCS_GUIDE

% Last Modified by GUIDE v2.5 19-Apr-2008 10:18:20

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @FCS_OpeningFcn, ...
                   'gui_OutputFcn',  @FCS_OutputFcn, ...
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


% --- Executes just before FLCS is made visible.
function FCS_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to FLCS (see VARARGIN)

% Choose default command line output for FLCS
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

global Bx By Bz; 
global FCS_MAX_Bx FCS_MAX_By FCS_MAX_Bz;
global Cx Cy Cz;
global Ax Ay Az;
global V;
global FCS_MAX_AIRSPEED;
global FCS_MAX_AIRSPEED_MAX;
global FCS_MAX_AIRSPEED_MIN;

FCS_MAX_AIRSPEED_MAX = 4300;
FCS_MAX_AIRSPEED_MIN = 1200;
FCS_MAX_AIRSPEED = 2100;
FCS_MAX_Bx       = 0.01;
FCS_MAX_By       = 0.01;
FCS_MAX_Bz       = 0.01;
Bx = 0.0025; 
By = 0.0018; 
Bz = 0.0025;
Cx = 1100.0; 
Cy = 550.0, 
Cz = 1300.5;
Ax = 0.8; 
Ay = 0.8; 
Az = 0.8;

V = FCS_MAX_AIRSPEED*0.2;
set(handles.max_airspeed_slider,'Value', FCS_MAX_AIRSPEED/(FCS_MAX_AIRSPEED_MAX - FCS_MAX_AIRSPEED_MIN));
set(handles.max_airspeed_edit,'String', num2str(FCS_MAX_AIRSPEED));
set(handles.airspeed_slider,'Value', 0.2);
set(handles.airspeed_edit,'String', num2str(V));

set(handles.Bx_slider,'Value', Bx/FCS_MAX_Bx);
set(handles.Bx_edit,'String', num2str(Bx));
set(handles.By_slider,'Value', By/FCS_MAX_By);
set(handles.By_edit,'String', num2str(By));
set(handles.Bz_slider,'Value', Bz/FCS_MAX_Bz);
set(handles.Bz_edit,'String', num2str(Bz));

set(handles.Cx_slider,'Value', Cx/FCS_MAX_AIRSPEED);
set(handles.Cx_edit,'String', num2str(Cx));
set(handles.Cy_slider,'Value', Cy/FCS_MAX_AIRSPEED);
set(handles.Cy_edit,'String', num2str(Cy));
set(handles.Cz_slider,'Value', Cz/FCS_MAX_AIRSPEED);
set(handles.Cz_edit,'String', num2str(Cz));


FCS_DrawLimiterFunction(handles);

% UIWAIT makes FLCS wait for user response (see UIRESUME)
% uiwait(handles.figure1);



% --- Outputs from this function are returned to the command line.
function varargout = FCS_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Draws the plot.
function FCS_DrawLimiterFunction(handles)
global Bx By Bz;
global Cx Cy Cz;
global Ax Ay Az;
global V;
global FCS_MAX_AIRSPEED;

K    = 0.5;
Xin = -1.0:0.1:1.0;

fBVx = K*Bx*( (V - Cx)./sqrt(1 + (Bx*(V - Cx)).^2) + Cx/sqrt(1 + (Bx*Cx).^2) );
X    = (1.0 - fBVx)*Xin;
Xout = Ax*X.^3 + (1.0 - Ax)*X;

fBVy = K*By*( (V - Cy)./sqrt(1 + (By*(V - Cy)).^2) + Cy/sqrt(1 + (By*Cy).^2) );
Y    = (1.0 - fBVy)*Xin;
Yout = Ay*Y.^3 + (1.0 - Ay)*Y;

fBVz = K*Bz*( (V - Cz)./sqrt(1 + (Bz*(V - Cz)).^2) + Cz/sqrt(1 + (Bz*Cz).^2) );
Z    = (1.0 - fBVz)*Xin;
Zout = Az*Z.^3 + (1.0 - Az)*Z;

% --- Draws limiter plot.
axes(handles.limiter_axes);
plot(Xin,Xout, Xin,Yout, Xin,Zout, 'LineWidth',1.5)
%%plot(Xin,Xout, Xin,Yout, Xin,Zout)
axis([-1.05 1.05 -1.1 1.1])
grid on
legend('Aileron (Xout)', 'Elevator (Yout)', 'Rudder (Zout)', 2)


% --- Draws fBv-parameter plot.
Vel = -1000.0:50.0:FCS_MAX_AIRSPEED;

fBVx = K*Bx*( (Vel - Cx)./sqrt(1 + (Bx*(Vel - Cx)).^2) + Cx/sqrt(1 + (Bx*Cx).^2) );

fBVy = K*By*( (Vel - Cy)./sqrt(1 + (By*(Vel - Cy)).^2) + Cy/sqrt(1 + (By*Cy).^2) );

fBVz = K*Bz*( (Vel - Cz)./sqrt(1 + (Bz*(Vel - Cz)).^2) + Cz/sqrt(1 + (Bz*Cz).^2) );

axes(handles.parameter_axes);
plot(Vel,fBVx, Vel,fBVy, Vel,fBVz, 'LineWidth',1.5)
axis([0 FCS_MAX_AIRSPEED -1.1 1.1])
grid on
legend('fBVx', 'fBVy', 'fBVz', 2)


% --- Executes on slider movement.
function max_airspeed_slider_Callback(hObject, eventdata, handles)
% hObject    handle to max_airspeed_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global FCS_MAX_AIRSPEED_MAX;
global FCS_MAX_AIRSPEED_MIN;
global FCS_MAX_AIRSPEED;
global Cx Cy Cz;

FCS_MAX_AIRSPEED = (FCS_MAX_AIRSPEED_MAX - FCS_MAX_AIRSPEED_MIN)*get(hObject,'Value') + FCS_MAX_AIRSPEED_MIN;
set(handles.max_airspeed_edit,'String', num2str(FCS_MAX_AIRSPEED));

set(handles.Cx_slider,'Value', Cx/FCS_MAX_AIRSPEED);
set(handles.Cy_slider,'Value', Cy/FCS_MAX_AIRSPEED);
set(handles.Cz_slider,'Value', Cz/FCS_MAX_AIRSPEED);

FCS_DrawLimiterFunction(handles);



% --- Executes during object creation, after setting all properties.
function max_airspeed_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to max_airspeed_slider (see GCBO)
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



function max_airspeed_edit_Callback(hObject, eventdata, handles)
% hObject    handle to max_airspeed_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of max_airspeed_edit as text
%        str2double(get(hObject,'String')) returns contents of max_airspeed_edit as a double


% --- Executes during object creation, after setting all properties.
function max_airspeed_edit_CreateFcn(hObject, eventdata, handles)
% hObject    handle to max_airspeed_edit (see GCBO)
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
function airspeed_slider_Callback(hObject, eventdata, handles)
% hObject    handle to airspeed_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global FCS_MAX_AIRSPEED;
global FCS_MAX_AIRSPEED_MAX;
global FCS_MAX_AIRSPEED_MIN;
global V;

V = FCS_MAX_AIRSPEED*get(hObject,'Value');
set(handles.airspeed_edit,'String', num2str(V));
FCS_DrawLimiterFunction(handles);

% --- Executes during object creation, after setting all properties.
function airspeed_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to airspeed_slider (see GCBO)
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




function airspeed_edit_Callback(hObject, eventdata, handles)
% hObject    handle to airspeed_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of airspeed_edit as text
%        str2double(get(hObject,'String')) returns contents of airspeed_edit as a double


% --- Executes during object creation, after setting all properties.
function airspeed_edit_CreateFcn(hObject, eventdata, handles)
% hObject    handle to airspeed_edit (see GCBO)
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
function Bx_slider_Callback(hObject, eventdata, handles)
% hObject    handle to Bx_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider
global FCS_MAX_Bx;
global Bx;

Bx    = FCS_MAX_Bx*get(hObject,'Value');
set(handles.Bx_edit,'String', num2str(Bx));
FCS_DrawLimiterFunction(handles);

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
global FCS_MAX_By;
global By;

By    = FCS_MAX_By*get(hObject,'Value');
set(handles.By_edit,'String', num2str(By));
FCS_DrawLimiterFunction(handles);


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
global FCS_MAX_Bz;
global Bz;

Bz    = FCS_MAX_Bz*get(hObject,'Value');
set(handles.Bz_edit,'String', num2str(Bz));
FCS_DrawLimiterFunction(handles);


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
global FCS_MAX_AIRSPEED;
global Cx;

Cx    = FCS_MAX_AIRSPEED*get(hObject,'Value');
set(handles.Cx_edit,'String', num2str(Cx));
FCS_DrawLimiterFunction(handles);


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
global FCS_MAX_AIRSPEED;
global Cy;

Cy    = FCS_MAX_AIRSPEED*get(hObject,'Value');
set(handles.Cy_edit,'String', num2str(Cy));
FCS_DrawLimiterFunction(handles);


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
global FCS_MAX_AIRSPEED;
global Cz;

Cz    = FCS_MAX_AIRSPEED*get(hObject,'Value');
set(handles.Cz_edit,'String', num2str(Cz));
FCS_DrawLimiterFunction(handles);


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





