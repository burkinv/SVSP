function varargout = HUD(varargin)
% HUD M-file for HUD.fig
%      HUD, by itself, creates a new HUD or raises the existing
%      singleton*.
%
%      H = HUD returns the handle to a new HUD or the handle to
%      the existing singleton*.
%
%      HUD('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in HUD.M with the given input arguments.
%
%      HUD('Property','Value',...) creates a new HUD or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before HUD_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to HUD_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Copyright 2002-2003 The MathWorks, Inc.

% Edit the above text to modify the response to help HUD

% Last Modified by GUIDE v2.5 28-Mar-2010 22:13:29

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @HUD_OpeningFcn, ...
                   'gui_OutputFcn',  @HUD_OutputFcn, ...
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


% --- Executes just before HUD is made visible.
function HUD_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to HUD (see VARARGIN)

% Choose default command line output for HUD
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

global alpha;
alpha = pi/6;

set(handles.slider1,'Value', alpha/(pi/2));
set(handles.edit1,'String', num2str(alpha));


% UIWAIT makes HUD wait for user response (see UIRESUME)
% uiwait(handles.figure1);
DrawAttittudeScale(hObject, eventdata, handles);


% --- Outputs from this function are returned to the command line.
function varargout = HUD_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


function DrawAttittudeScale(hObject, eventdata, handles)

global alpha;
h = 1.85;
r = 2.5;
w = 20;
d = 8;


beta  = alpha/2 + pi/4;
theta = pi/4 - alpha/2;
len   = w/cos(alpha);
a     = len/7;


w_tan_alpha  = w*tan(alpha);
h2_tan_theta = (h/2)*tan(theta);
a_cos_alpha  = a*cos(alpha);
a_sin_alpha  = a*sin(alpha);

h_sin_alpha  = h*sin(alpha);
h_cos_alpha  = h*cos(alpha);
h2_sin_alpha = h_sin_alpha/2;
h2_cos_alpha = h_cos_alpha/2;

w_cos_alpha = w*cos(alpha);
w_sin_alpha = w*sin(alpha);

% %Climb range
x(1) = -d + h/2;                             y(1) = -w_tan_alpha - r;
x(2) = -d - h/2;                             y(2) = y(1);
x(3) = x(2);                                 y(3) = -w_tan_alpha - h2_tan_theta;

x(4) = x(1);                                 y(4) = y(1);
x(5) = x(3);                                 y(5) = y(3);
x(6) = x(1);                                 y(6) = -w_tan_alpha + h2_tan_theta;

x(7) = x(3);                                 y(7) = y(3);
x(8) = -d - w_cos_alpha - h2_sin_alpha;      y(8) = -w_tan_alpha + w_sin_alpha - h2_cos_alpha;
x(9) = -d - w_cos_alpha + h2_sin_alpha;      y(9) = -w_tan_alpha + w_sin_alpha + h2_cos_alpha;

x(10)= x(3);                                 y(10)= y(3);
x(11)= x(9);                                 y(11)= y(9);
x(12)= x(6);                                 y(12)= y(6);

for i = 1:12
    x(12 + i) = -x(i); y(12 + i) = y(i);
    z(i) = 0;          z(2*i) = 0;
    c(i) = 55;         c(2*i) = 55;
    
end

TRI = [1 2 3; 4 5 6; 7 8 9; 10 11 12; 13 14 15; 16 17 18; 19 20 21; 22 23 24]; 

axes(handles.HUD);   
%triplot(TRI,x,y,'red', 'linewidth', 1);
trisurf(TRI,x,y,z,c);
axis([-40 40 -40 40]);

% Diving range
% x(1) = -d + h/2;                             y(1) = w_tan_alpha + r;
% x(2) = -d - h/2;                             y(2) = y(1);
% x(3) = x(2);                                 y(3) = w_tan_alpha + h2_tan_theta;
% 
% x(4) = x(1);                                 y(4) = y(1);
% x(5) = x(3);                                 y(5) = y(3);
% x(6) = x(1);                                 y(6) = w_tan_alpha - h2_tan_theta;
% 
% x(7) = x(3);                                 y(7) = y(3);
% x(8) = -d - a_cos_alpha - h2_sin_alpha;      y(8) = w_tan_alpha - a_sin_alpha + h2_cos_alpha;
% x(9) = -d - a_cos_alpha + h2_sin_alpha;      y(9) = w_tan_alpha - a_sin_alpha - h2_cos_alpha;
% 
% x(10)= x(3);                                 y(10)= y(3);
% x(11)= x(9);                                 y(11)= y(9);
% x(12)= x(6);                                 y(12)= y(6);
% 
% x(13)= x(8)  - a_cos_alpha;                  y(13)= y(8)  - a_sin_alpha;
% x(14)= x(13) - a_cos_alpha;                  y(14)= y(13) - a_sin_alpha;
% x(15)= x(14) + h_sin_alpha;                  y(15)= y(14) - h_cos_alpha;
% 
% x(16)= x(13);                                y(16)= y(13);
% x(17)= x(15);                                y(17)= y(15);
% x(18)= x(13) + h_sin_alpha;                  y(18)= y(13) - h_cos_alpha;
% 
% x(19)= x(14) - a_cos_alpha;                  y(19)= y(14) - a_sin_alpha;
% x(20)= x(19) - a_cos_alpha;                  y(20)= y(19) - a_sin_alpha;
% x(21)= x(20) + h_sin_alpha;                  y(21)= y(20) - h_cos_alpha;
% 
% x(22)= x(19);                                y(22)= y(19);
% x(23)= x(21);                                y(23)= y(21);
% x(24)= x(19) + h_sin_alpha;                  y(24)= y(19) - h_cos_alpha;
% 
% x(25)= x(20) - a_cos_alpha;                  y(25)= y(20) - a_sin_alpha;
% x(26)= x(25) - a_cos_alpha;                  y(26)= y(25) - a_sin_alpha;
% x(27)= x(26) + h_sin_alpha;                  y(27)= y(26) - h_cos_alpha;
% 
% x(28)= x(25);                                y(28)= y(25);
% x(29)= x(27);                                y(29)= y(27);
% x(30)= x(25) + h_sin_alpha;                  y(30)= y(25) - h_cos_alpha;
% 
% for i = 1:30
%     x(30 + i) = -x(i); y(30 + i) = y(i);
%     z(i) = 0;          z(2*i) = 0;
%     c(i) = 55;          c(2*i) = 55;
%     
% end
% 
% TRI = [1 2 3; 4 5 6; 7 8 9; 10 11 12; 13 14 15; 
%        16 17 18; 19 20 21; 22 23 24; 25 26 27; 28 29 30;
%        31 32 33; 34 35 36; 37 38 39; 40 41 42; 43 44 45; 
%        46 47 48; 49 50 51; 52 53 54; 55 56 57; 58 59 60]; 

axes(handles.HUD);   
%triplot(TRI,x,y,'red', 'linewidth', 1);
trisurf(TRI,x,y,z,c);

tx = [-d - 1.2*w, d + 1.2*w];
ty = [0,      0    ];
text(tx, ty, int2str(180*alpha/pi), 'FontSize', 20, 'FontWeight', 'bold', 'HorizontalAlignment', 'center');


axis([-40 40 -40 40]);
grid on;



% --- Executes on slider movement.
function slider1_Callback(hObject, eventdata, handles)
% hObject    handle to slider1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider

global alpha;

alpha    = (pi/4)*get(hObject,'Value');
set(handles.edit1,'String', num2str(alpha));
DrawAttittudeScale(hObject, eventdata, handles); 



% --- Executes during object creation, after setting all properties.
function slider1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to slider1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end



function edit1_Callback(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit1 as text
%        str2double(get(hObject,'String')) returns contents of edit1 as a double


% --- Executes during object creation, after setting all properties.
function edit1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


