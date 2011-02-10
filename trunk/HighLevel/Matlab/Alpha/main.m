function varargout = main(varargin)
% MAIN M-file for main.fig
%      MAIN, by itself, creates a new MAIN or raises the existing
%      singleton*.
%
%      H = MAIN returns the handle to a new MAIN or the handle to
%      the existing singleton*.
%
%      MAIN('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in MAIN.M with the given input arguments.
%
%      MAIN('Property','Value',...) creates a new MAIN or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before main_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to main_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help main

% Last Modified by GUIDE v2.5 01-Feb-2011 13:29:57

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @main_OpeningFcn, ...
                   'gui_OutputFcn',  @main_OutputFcn, ...
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


% --- Executes just before main is made visible.
function main_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to main (see VARARGIN)

% Choose default command line output for main
handles.output = hObject;

handles.filtersize=9;

handles.fileformat='jpg';

handles.sourcefoldername='PEXP_Source';

handles.destinationfoldername= 'PEXP_Result';

handles.listoffiles='';

handles.category_check=0;

handles.category_list=0;

handles.alphadir='C:\matlab\Alpha\';% change to current working directory

cd(handles.alphadir);

handles.parameterfile=[];

D = dir;

foundsource=0;

for i = 1:length(D)
    
if strcmp(D(i).name,handles.sourcefoldername)

    foundsource=1;
    
end

end

    if foundsource
        
        cd(handles.sourcefoldername);
        
        % detect directories
D= dir;

count = 0;

for i = 3:length(D)
    
    if (D(i).isdir)

        count = count +1;
        
        switch count
                       
            case 1
                
                set(handles.Tick_1,'String',D(i).name);
               case 2
                
                set(handles.Tick_2,'String',D(i).name);
                
                 case 3
                
                set(handles.Tick_3,'String',D(i).name);
                
                 case 4
                
                set(handles.Tick_4,'String',D(i).name);
                
                 case 5
                
                set(handles.Tick_5,'String',D(i).name);
                
            otherwise
                
                disp('Limiting to first 5 categories');
       end
     end
end
      

    else
        
          errordlg('Source directory not detected');
      
    end
    
 % Update handles structure
guidata(hObject, handles);



% UIWAIT makes main wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = main_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in Tick_1.
function Tick_1_Callback(hObject, eventdata, handles)
% hObject    handle to Tick_1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of Tick_1


% --- Executes on button press in Tick_2.
function Tick_2_Callback(hObject, eventdata, handles)
% hObject    handle to Tick_2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of Tick_2


% --- Executes on button press in Tick_3.
function Tick_3_Callback(hObject, eventdata, handles)
% hObject    handle to Tick_3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of Tick_3


% --- Executes on button press in Tick_4.
function Tick_4_Callback(hObject, eventdata, handles)
% hObject    handle to Tick_4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of Tick_4


% --- Executes on button press in Tick_5.
function Tick_5_Callback(hObject, eventdata, handles)
% hObject    handle to Tick_5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of Tick_5



function Txt_sig_step_size_Callback(hObject, eventdata, handles)
% hObject    handle to Txt_sig_step_size (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Txt_sig_step_size as text
%        str2double(get(hObject,'String')) returns contents of Txt_sig_step_size as a double
input = str2num(get(hObject,'String'));

if ((isempty(input))||(input<=0))
     set(hObject,'String','1')
else
    if (input>5)
               set(hObject,'String','5')
    end
    end
guidata(hObject, handles);

% --- Executes during object creation, after setting all properties.
function Txt_sig_step_size_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Txt_sig_step_size (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in Button_generate.
function Button_generate_Callback(hObject, eventdata, handles)
% hObject    handle to Button_generate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.Button_generate,'Enable','off');
handles.listoffiles='';
handles.category_list=0;

cd(handles.alphadir);

handles.parameterfile=fopen('Parameter.txt','w');

inc = str2num(get(handles.Txt_sig_step_size,'String'));

fprintf(handles.parameterfile,'step_size,%d \r\n',inc);

handles.category_check=1:5;

for i = 1:5
    
cd(strcat(handles.alphadir,handles.sourcefoldername));

switch i
    
    case 1
        check=get(handles.Tick_1,'Value');
        sub_dir = get(handles.Tick_1,'String');
    case 2
        check=get(handles.Tick_2,'Value');
        sub_dir = get(handles.Tick_2,'String');
    case 3
        check=get(handles.Tick_3,'Value');
        sub_dir = get(handles.Tick_3,'String');
        
    case 4
        check=get(handles.Tick_4,'Value');
        sub_dir = get(handles.Tick_4,'String');
    case 5
        check=get(handles.Tick_5,'Value');
        sub_dir = get(handles.Tick_5,'String');
end

if (check)
        
    x=0;
    while(~x)
        x=(floor(length(handles.category_check)*rand));
    end

    handles.category_check=setdiff(handles.category_check,handles.category_check(x));
    
    fprintf(handles.parameterfile,'%s,%d \r\n',sub_dir,handles.category_check(x));
    
   cd(sub_dir);
   
  [handles.listoffiles handles.category_list]=process_sub_dir(handles,inc,handles.category_check(x));
    
       guidata(hObject, handles);
  
       cd(handles.alphadir);
end


end



cd(strcat(handles.alphadir,handles.destinationfoldername));

counter = 0 ;

array = 1:length(handles.listoffiles);



 while(~isequal(array,[]))
    
    counter = counter +1;
    
   number = ceil(length(array)*rand);
   
   
   movefile(handles.listoffiles(array(number),:),strcat(num2str(counter),'.',handles.fileformat));
    
   write_format= [(counter) handles.category_list(array(number),1) handles.category_list(array(number),2) handles.category_list(array(number),3)];
   
  fprintf(handles.parameterfile,'%d,%d,%d,%d \r\n',write_format);
    
  array=setdiff(array,array(number));
  
 end
 

fclose(handles.parameterfile);

cd(handles.alphadir);

msgbox('All done!');

set(handles.Button_generate,'Enable','on');




function [LOF CL]=process_sub_dir(handles,inc,cate)

LOF = handles.listoffiles;
CL = handles.category_list;

list = dir(strcat('*.',handles.fileformat));


for i = 1:length(list)
    
    I = imread(list(i).name);  
        
    if (length(size(I))>2)
    
    I = double(rgb2gray(I));
    
    end
    
    for j = 1:7
        
        Tval =(2^((j-1)*inc));
        
    H = fspecial('gaussian',handles.filtersize,Tval);    
        
    
     J = imfilter(I,H,'conv','replicate');    
    
    processedfilename = strcat(list(i).name,'_proc_',num2str(j),'_',handles.fileformat);
    
    imwrite(uint8(J),processedfilename,handles.fileformat);
         
    movefile(processedfilename,strcat(handles.alphadir,handles.destinationfoldername));
         
  
    LOF =strvcat(LOF,processedfilename);
    
   
     CL(size(LOF,1),1) = cate;
    
    CL(size(LOF,1),2) = i;
    
    CL(size(LOF,1),3) = j;
    
    end

    
end
  

% --- Executes on button press in Button_exit.
function Button_exit_Callback(hObject, eventdata, handles)
% hObject    handle to Button_exit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

close all;
