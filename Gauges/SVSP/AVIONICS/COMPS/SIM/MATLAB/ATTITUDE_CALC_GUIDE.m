function varargout = ATTITUDE_CALC_GUIDE(varargin)
% ATTITUDE_CALC_GUIDE M-file for ATTITUDE_CALC_GUIDE.fig
%      ATTITUDE_CALC_GUIDE, by itself, creates a new ATTITUDE_CALC_GUIDE or raises the existing
%      singleton*.
%
%      H = ATTITUDE_CALC_GUIDE returns the handle to a new ATTITUDE_CALC_GUIDE or the handle to
%      the existing singleton*.
%
%      ATTITUDE_CALC_GUIDE('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in ATTITUDE_CALC_GUIDE.M with the given input arguments.
%
%      ATTITUDE_CALC_GUIDE('Property','Value',...) creates a new ATTITUDE_CALC_GUIDE or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before ATTITUDE_CALC_GUIDE_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to ATTITUDE_CALC_GUIDE_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Copyright 2002-2003 The MathWorks, Inc.

% Edit the above text to modify the response to help ATTITUDE_CALC_GUIDE

% Last Modified by GUIDE v2.5 16-Aug-2008 10:40:27

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @ATTITUDE_CALC_GUIDE_OpeningFcn, ...
                   'gui_OutputFcn',  @ATTITUDE_CALC_GUIDE_OutputFcn, ...
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


% --- Executes just before ATTITUDE_CALC_GUIDE is made visible.
function ATTITUDE_CALC_GUIDE_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to ATTITUDE_CALC_GUIDE (see VARARGIN)

% Choose default command line output for ATTITUDE_CALC_GUIDE
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes ATTITUDE_CALC_GUIDE wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = ATTITUDE_CALC_GUIDE_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

function ATTITUDE_CALC_GUIDE_SetAngles(handles)
Vx = str2double(get(handles.X1,'String')) - str2double(get(handles.X2,'String'));
Vy = str2double(get(handles.Y1,'String')) - str2double(get(handles.Y2,'String'));
Vz = str2double(get(handles.Z1,'String')) - str2double(get(handles.Z2,'String'));

lengthXY = sqrt(Vx^2 + Vy^2);
length   = sqrt(Vx^2 + Vy^2 + Vz^2);

if (length > 0)    
    Sin_ThetaY = -Vz/length;
    Cos_ThetaZ = Vx/lengthXY;
    
    ThetaY = (asin(Sin_ThetaY)/pi)*180.0;
    ThetaZ = (acos(Cos_ThetaZ)/pi)*180.0;
    
    set(handles.ThetaY,'String', num2str(ThetaY));
    set(handles.ThetaZ,'String', num2str(ThetaZ));
    
end

function X1_Callback(hObject, eventdata, handles)
% hObject    handle to X1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of X1 as text
%        str2double(get(hObject,'String')) returns contents of X1 as a double
ATTITUDE_CALC_GUIDE_SetAngles(handles);


% --- Executes during object creation, after setting all properties.
function X1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to X1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


function Y1_Callback(hObject, eventdata, handles)
% hObject    handle to Y1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Y1 as text
%        str2double(get(hObject,'String')) returns contents of Y1 as a double
ATTITUDE_CALC_GUIDE_SetAngles(handles);

% --- Executes during object creation, after setting all properties.
function Y1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Y1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function Z1_Callback(hObject, eventdata, handles)
% hObject    handle to Z1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Z1 as text
%        str2double(get(hObject,'String')) returns contents of Z1 as a double
ATTITUDE_CALC_GUIDE_SetAngles(handles);


% --- Executes during object creation, after setting all properties.
function Z1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Z1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function X2_Callback(hObject, eventdata, handles)
% hObject    handle to X2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of X2 as text
%        str2double(get(hObject,'String')) returns contents of X2 as a double
ATTITUDE_CALC_GUIDE_SetAngles(handles);


% --- Executes during object creation, after setting all properties.
function X2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to X2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


function Y2_Callback(hObject, eventdata, handles)
% hObject    handle to Y2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Y2 as text
%        str2double(get(hObject,'String')) returns contents of Y2 as a double
ATTITUDE_CALC_GUIDE_SetAngles(handles);


% --- Executes during object creation, after setting all properties.
function Y2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Y2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function Z2_Callback(hObject, eventdata, handles)
% hObject    handle to Z2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Z2 as text
%        str2double(get(hObject,'String')) returns contents of Z2 as a double
ATTITUDE_CALC_GUIDE_SetAngles(handles);


% --- Executes during object creation, after setting all properties.
function Z2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Z2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function ThetaY_Callback(hObject, eventdata, handles)
% hObject    handle to ThetaY (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ThetaY as text
%        str2double(get(hObject,'String')) returns contents of ThetaY as a double


% --- Executes during object creation, after setting all properties.
function ThetaY_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ThetaY (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function ThetaZ_Callback(hObject, eventdata, handles)
% hObject    handle to ThetaZ (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ThetaZ as text
%        str2double(get(hObject,'String')) returns contents of ThetaZ as a double


% --- Executes during object creation, after setting all properties.
function ThetaZ_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ThetaZ (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end



function ThetaX_Callback(hObject, eventdata, handles)
% hObject    handle to ThetaX (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ThetaX as text
%        str2double(get(hObject,'String')) returns contents of ThetaX as a double


% --- Executes during object creation, after setting all properties.
function ThetaX_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ThetaX (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


