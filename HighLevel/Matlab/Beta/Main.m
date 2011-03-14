function varargout = Main(varargin)
% MAIN M-file for Main.fig
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
%      applied to the GUI before Main_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to Main_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help Main

% Last Modified by GUIDE v2.5 08-Mar-2011 10:10:37

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @Main_OpeningFcn, ...
                   'gui_OutputFcn',  @Main_OutputFcn, ...
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


% --- Executes just before Main is made visible.
function Main_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to Main (see VARARGIN)

% Choose default command line output for Main
handles.output = hObject;

handles.filtersize=9;

handles.basesigma=sqrt(2); %%%%

handles.inputformat='jpg';

handles.outputformat='jpg';

handles.sourcefoldername='PEXP_Source';

handles.betadir='C:\matlab\Beta\';% change to current working directory

handles.resultfoldername= 'PEXP_Result';

handles.mode=set_mode();

handles.blankpage=-1;

handles.resolution = set_resolution();

handles.ms = set_ms();

handles.c_list=[];

handles.listoffiles=[];

handles.doc=[];

set(handles.dm_reso,'String',handles.resolution.list);
% Update handles structure
guidata(hObject, handles);

% UIWAIT makes Main wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = Main_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in lb_main.
function lb_main_Callback(hObject, eventdata, handles)
% hObject    handle to lb_main (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns lb_main contents as cell array
%        contents{get(hObject,'Value')} returns selected item from lb_main


% --- Executes during object creation, after setting all properties.
function lb_main_CreateFcn(hObject, eventdata, handles)
% hObject    handle to lb_main (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pb_init.
function pb_init_Callback(hObject, eventdata, handles)
% hObject    handle to pb_init (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
cd(strcat(handles.betadir,handles.sourcefoldername,'\'));

D = dir;

list = '';

for i = 3:length(D)
    
if (D(i).isdir)
    list = strvcat(list,D(i).name);    
    
end
      
end

set(handles.dm_mode,'Value',1);

set(handles.lb_main,'String',list);

cd(handles.betadir);

set(handles.dm_mode,'String',handles.mode.list);

set(handles.dm_mode,'Enable','On');

 set(handles.txt_param1,'String','IteMin');

        set(handles.txt_param2,'String','IteMax');

set(handles.txt_param1,'Visible','On');

set(handles.txt_param2,'Visible','On');

set(handles.pb_init,'Enable','Off');

set(handles.pb_go,'Enable','On');

handles.blankpage = get_page(handles);

set(handles.rb_black,'Value',0);

set(handles.rb_random,'Value',0);

handles.resolution.current = get_resolution(handles);

handles.ms.current = get_ms(handles);



guidata(hObject, handles);
% --- Executes on selection change in dm_mode.
function dm_mode_Callback(hObject, eventdata, handles)
% hObject    handle to dm_mode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

pos = get(handles.dm_mode,'Value');

switch(pos)
    
    case 1
        
        set(handles.txt_param1,'String','IteMin');

        set(handles.txt_param2,'String','IteMax');
        
        set(handles.txt_param1,'Visible','On');

        set(handles.txt_param2,'Visible','On');

        set(handles.pb_init,'Enable','Off');

    case 2
        
        set(handles.txt_param1,'String','LvlMin');

        set(handles.txt_param2,'String','LvlMax');
        
        set(handles.txt_param1,'Visible','On');

        set(handles.txt_param2,'Visible','On');

        set(handles.pb_init,'Enable','Off');
        
        
    case 3
        
        
end
        

% Hints: contents = cellstr(get(hObject,'String')) returns dm_mode contents as cell array
%        contents{get(hObject,'Value')} returns selected item from dm_mode


% --- Executes during object creation, after setting all properties.
function dm_mode_CreateFcn(hObject, eventdata, handles)
% hObject    handle to dm_mode (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function txt_param1_Callback(hObject, eventdata, handles)
% hObject    handle to txt_param1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txt_param1 as text
%        str2double(get(hObject,'String')) returns contents of txt_param1 as a double


% --- Executes during object creation, after setting all properties.
function txt_param1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txt_param1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function txt_param2_Callback(hObject, eventdata, handles)
% hObject    handle to txt_param2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txt_param2 as text
%        str2double(get(hObject,'String')) returns contents of txt_param2 as a double


% --- Executes during object creation, after setting all properties.
function txt_param2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txt_param2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pb_go.
function pb_go_Callback(hObject, eventdata, handles)
% hObject    handle to pb_go (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
    
set(hObject,'Enable','Off');

handles.listoffiles=[];

cat_list= select_category(handles);

mode=get(handles.dm_mode,'Value');

[params, error] = get_params(handles,mode);


if (~error(1))&&(~error(2))
    
    handles.c_list= randperm(size(cat_list,1));
    
    switch(mode)
        
        case 1
             
            
        [handles.listoffiles]=process_mode_A(handles,cat_list,params);
               
        handles.listoffiles= process_doc(handles,cat_list,params,mode);
        
        process_list_A(handles);
        
        case 2
            
        [handles.listoffiles]=process_mode_B(handles,cat_list,params);
               
        handles.listoffiles= process_doc(handles,cat_list,params,mode);
        
        process_list_B(handles);   
            
            
        case 3
            
            
    end
        
    
    msgbox('All done!');
    
    set(hObject,'Enable','On');
    
        else
               errordlg('Param Error');
               
               set(hObject,'Enable','On');
               
               
end



function [a,err] = get_params(handles,mode)

input=0;
input(1) = get_txt_param(handles,1);
input(2) = get_txt_param(handles,2);

for i = 1:handles.mode.numparams(mode)
    
 if (input(i)<=0)
     
     txt_param_set(handles,i,'Invalid');
     
          err(i)=true;
          
   elseif (input(i)>handles.mode.paramax(i,mode))
        
        txt_param_set(handles,i,num2str(handles.mode.paramax(i,mode)));
        
        err(i) = true;
 else
     
     err(i) = false; %#ok<*AGROW>
 end
    
end


a(1)=input(1);
a(2)=input(2);




function [out] = get_txt_param(handles,i)

switch i
    
    case 1
        p = str2num(get(handles.txt_param1,'String'));
        
    case 2
        p = str2num(get(handles.txt_param2,'String')); %#ok<*ST2NM>
    case 3
        p = str2num(get(handles.txt_expt,'String'));
        
end

if isequal(p,[])
p=-1;    
end

out = p;



function [out]=select_category(handles)

a = get(handles.lb_main,'String');

b = get(handles.lb_main,'Value');

out=a(b,:);



function txt_param_set(handles, num, string)
switch num
    case 1
        
    set(handles.txt_param1,'String',string)
    
    case 2
        
    set(handles.txt_param2,'String',string)
        
end


function [out]= set_mode()
temp =[];

temp.list = ('Gaussian Blur');
temp.list = strvcat(temp.list,'Grayscale Level');


temp.numparams = [2 2];

temp.paramax(1,:)= [20 7];
temp.paramax(2,:)= [50 8];

out = temp;

function [out]=set_resolution()

temp = [];
temp.list= ('240 x 320');
temp.value = [240 320];
temp.current=[];
out= temp;


function clean_current_dir()

D= dir;

if size(D,1)>2

    for i = 3:size(D,1)
        
        delete(D(i).name);
        
    end
    
    
end


function [out]= process_doc(handles,cat_list,params,m)


cd(handles.betadir);

paramfile=fopen('Parameters.txt','w');

%print exp params here
fprintf(paramfile,'0;%d \r\n',handles.ms.current);
fprintf(paramfile,'1;%d \r\n',handles.resolution.current-1);
fprintf(paramfile,'2;%d \r\n',handles.blankpage);

%category info

fprintf(paramfile,'3;%d \r\n',size(cat_list,1));

for i = 1:size(cat_list,1)
    
fprintf(paramfile,'%s,%d \r\n',cat_list(i,1:end),handles.c_list(i));

end

%print file params here
fprintf(paramfile,'4;%s \r\n',handles.mode.list(m,:));
fprintf(paramfile,'4;param1,%d \r\n',params(1));
fprintf(paramfile,'4;param2,%d \r\n',params(2));



%file list

f_list = randperm(size(handles.listoffiles,2));

fprintf(paramfile,'5;%d \r\n',length(f_list));

new_list=handles.listoffiles;

for i = 1:length(f_list)
   new_list(i)=handles.listoffiles(f_list(i));
   
    fprintf(paramfile,'%d,%d,%d,%d,%s \r\n',i,new_list(i).category,new_list(i).param1,new_list(i).param2,new_list(i).name);
    
        
end

fclose(paramfile);
out = new_list;



%MODE A
function [lof] =process_mode_A(handles,list,params)

for i = 1:size(list,1)
    
       
cd(strcat(handles.betadir,handles.sourcefoldername));

cd(list(i,:));
    
    locallist= dir(strcat('*.',handles.inputformat));
    
   
    
    for j = 1:length(locallist)
      
        index=size(handles.listoffiles,2)+1;
        
        handles.listoffiles(index).source = strcat(handles.betadir,handles.sourcefoldername,'\',list(i,:),'\',locallist(j).name);
        
        handles.listoffiles(index).param1 =ceil(rand*(params(2)-params(1)))+params(1);
        
        handles.listoffiles(index).param2 =-1;
        
        handles.listoffiles(index).category = handles.c_list(i);
        
        handles.listoffiles(index).name = locallist(j).name;
               
                   
    end
                 
end

cd(handles.betadir);

lof = handles.listoffiles;

function process_list_A(handles)

cd(strcat(handles.betadir,handles.resultfoldername,'\'));

clean_current_dir();


for i = 1:size(handles.listoffiles,2)
    
    I = imread(handles.listoffiles(i).source);
    
    if (length(size(I))>2)
    
    I = double(rgb2gray(I));
    
    end
    
    
    I = imresize(I,handles.resolution.value(handles.resolution.current,:),'bilinear');
    
     H = fspecial('gaussian',handles.filtersize,handles.basesigma); 
    
     
     
     for j = 1:handles.listoffiles(i).param1
         
     I = imfilter(I,H,'conv','replicate'); 
     
     end
     
        
     
 
    processedfilename = strcat(num2str(i),'.',handles.outputformat);
    
     imwrite(uint8(I),processedfilename,handles.outputformat); 
    
    
end

cd(handles.betadir);
%END MODE A

%MODE B
function [lof] =process_mode_B(handles,list,params)

for i = 1:size(list,1)
    
       
cd(strcat(handles.betadir,handles.sourcefoldername));

cd(list(i,:));
    
    locallist= dir(strcat('*.',handles.inputformat));
    
   
    
    for j = 1:length(locallist)
      
        index=size(handles.listoffiles,2)+1;
        
        handles.listoffiles(index).source = strcat(handles.betadir,handles.sourcefoldername,'\',list(i,:),'\',locallist(j).name);
        
           handles.listoffiles(index).param1 =round(rand*(params(2)-params(1)))+params(1);
        
        handles.listoffiles(index).param2 =-1;
        
        handles.listoffiles(index).category = handles.c_list(i);
        
        handles.listoffiles(index).name = locallist(j).name;
               
                   
    end
                 
end

cd(handles.betadir);

lof = handles.listoffiles;

function process_list_B(handles)

cd(strcat(handles.betadir,handles.resultfoldername,'\'));

clean_current_dir();

for i = 1:size(handles.listoffiles,2)
    
    I = imread(handles.listoffiles(i).source);
    
    if (length(size(I))>2)
    
    I = double(rgb2gray(I));
    
    end
    
    I = imresize(I,handles.resolution.value(handles.resolution.current,:),'bilinear');
    
    val=255/(handles.listoffiles(i).param1-1);
    
    J = round(I/val)*val;
     
    processedfilename = strcat(num2str(i),'.',handles.outputformat);
    
     imwrite(uint8(J),processedfilename,handles.outputformat); 
    
    
end

cd(handles.betadir);
%END MODE B


% --- Executes on button press in pb_exit.
function pb_exit_Callback(hObject, eventdata, handles) %#ok<INUSD>
% hObject    handle to pb_exit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close all;



function txt_expt_Callback(hObject, eventdata, handles) %#ok<INUSD>
% hObject    handle to txt_expt (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of txt_expt as text
%        str2double(get(hObject,'String')) returns contents of txt_expt as a double


% --- Executes during object creation, after setting all properties.
function txt_expt_CreateFcn(hObject, eventdata, handles)
% hObject    handle to txt_expt (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in dm_reso.
function dm_reso_Callback(hObject, eventdata, handles)
% hObject    handle to dm_reso (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns dm_reso contents as cell array
%        contents{get(hObject,'Value')} returns selected item from dm_reso


% --- Executes during object creation, after setting all properties.
function dm_reso_CreateFcn(hObject, eventdata, handles)
% hObject    handle to dm_reso (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in rb_black.
function rb_black_Callback(hObject, eventdata, handles)
% hObject    handle to rb_black (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

set(handles.rb_random,'Value',0);

[err]=check_ms(handles);

if err
    set(handles.rb_black,'Value',0);
    set(handles.pb_init,'Enable','Off');
    errordlg('Invalid Exposure Time');
       
else
    set(handles.pb_init,'Enable','On');
    set(handles.pb_go,'Enable','Off');
end


    
% Hint: get(hObject,'Value') returns toggle state of rb_black


% --- Executes on button press in rb_random.
function rb_random_Callback(hObject, eventdata, handles)
% hObject    handle to rb_random (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.rb_black,'Value',0);

[err]=check_ms(handles);

if err
    set(handles.rb_random,'Value',0);
    
    set(handles.pb_init,'Enable','Off');
    errordlg('Invalid Exposure Time');
    
else
    
    set(handles.pb_init,'Enable','On');
    set(handles.pb_go,'Enable','Off');
end




% Hint: get(hObject,'Value') returns toggle state of rb_random

function [out] = check_ms(handles)


input = get_txt_param(handles,3);
 
 if (input<=0)
     
     set(handles.txt_expt,'String','Invalid');
     
          err=true;
          
   elseif (input>handles.ms.max)
        
        set(handles.txt_expt,'String',num2str(handles.ms.max));
        
        err = true;
 else
     
     err = false;
 end
    
 out=err;

function [out]=set_ms()
        
out.current =0;
out.max=100;
 
function [out] = get_ms(handles)

out  = get_txt_param(handles,3);



function [out] = get_resolution(handles)

out = get(handles.dm_reso,'Value');


function [out] = get_page(handles)

a = get(handles.rb_black,'Value');
b = get(handles.rb_random,'Value');

if isequal(a,b,0)
    
    out=-1;
    errordlg('Invalid blank page');
    
else
    if (a)
        out = 0;
    else
        out=1;
    end
     
end

        
        