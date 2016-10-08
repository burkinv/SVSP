function varargout = SeekerRotation(varargin)
% SEEKERROTATION M-file for SeekerRotation.fig
%      SEEKERROTATION, by itself, creates a new SEEKERROTATION or raises the existing
%      singleton*.
%
%      H = SEEKERROTATION returns the handle to a new SEEKERROTATION or the handle to
%      the existing singleton*.
%
%      SEEKERROTATION('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in SEEKERROTATION.M with the given input arguments.
%
%      SEEKERROTATION('Property','Value',...) creates a new SEEKERROTATION or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before SeekerRotation_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to SeekerRotation_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help SeekerRotation

% Last Modified by GUIDE v2.5 28-Jan-2010 21:53:30

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @SeekerRotation_OpeningFcn, ...
                   'gui_OutputFcn',  @SeekerRotation_OutputFcn, ...
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


% --- Executes just before SeekerRotation is made visible.
function SeekerRotation_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to SeekerRotation (see VARARGIN)

% Choose default command line output for SeekerRotation
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes SeekerRotation wait for user response (see UIRESUME)
% uiwait(handles.figure1);


MAX_AQUISITION_ANGLE_DEGREES = 45;
SEEKER_ROTATION_SPEED        = 500;

x = -MAX_AQUISITION_ANGLE_DEGREES:.5:MAX_AQUISITION_ANGLE_DEGREES;

A = 0.7;
C = 5;

cosx = cos(pi*(x - C)/180);
sinx = sin(pi*(x - C)/180);

y = 1 - 1./(1 + SEEKER_ROTATION_SPEED*(1 - cosx));
y1 = A*((x - C)./sqrt(1 + 40*(x - C).^2));

plot(x, y, x, y1, 'LineWidth',1.5);
axis([-MAX_AQUISITION_ANGLE_DEGREES MAX_AQUISITION_ANGLE_DEGREES -1.2 1.2]);
grid on


% --- Outputs from this function are returned to the command line.
function varargout = SeekerRotation_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;
