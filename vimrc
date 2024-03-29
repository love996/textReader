set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')

" let Vundle manage Vundle, required
Bundle 'gmarik/Vundle.vim'
" Plugin 'tpope/vim-fugitive'
Plugin 'ervandew/supertab'
Plugin 'scrooloose/nerdtree'
Plugin 'majutsushi/tagbar'
Plugin 'fholgado/minibufexpl.vim'
Plugin 'vim-scripts/OmniCppComplete'
Plugin 'flazz/vim-colorschemes'
Plugin 'rking/ag.vim'  "https://geoff.greer.fm/ag/releases/the_silver_searcher-1.0.2.tar.gz
Plugin 'Valloric/ListToggle'
Plugin 'fatih/vim-go' " :GoInstallBinaries
Plugin 'python.vim'
Plugin 'nvie/vim-flake8'
Plugin 'tell-k/vim-autopep8'
Plugin 'nsf/gocode', {'rtp': 'vim/'}


" All of your Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required
" To ignore plugin indent changes, instead use:
"filetype plugin on
"
" Brief help
" :PluginList       - lists configured plugins
" :PluginInstall    - installs plugins; append `!` to update or just
" :PluginUpdate
" :PluginSearch foo - searches for foo; append `!` to refresh local cache
" :PluginClean      - confirms removal of unused plugins; append `!` to auto-approve removal
" Put your non-Plugin stuff after this line

set tags=$PROJECT_CBA_TAGS
let s:currentPlatform=0
set statusline=[%n]\ [CBA]\ %F%m%r%h%w\ [%l:%c]\ %y
set nofoldenable

function RegenTags()
   silent !clear
   execute "!ctaggen2.sh -r"
endfunction

function OpenFileSwitch()
    let s:newfilext=expand("%:e")
    if s:newfilext=="cpp"
        execute "tag ".expand("%:t:r").".h"
    elseif s:newfilext=="h"
        execute "tag ".expand("%:t:r").".cpp"
    else
        echo "Can not find relation ".s:newfilext
    endif
endfunction

set ruler
set number
set novisualbell
set noerrorbells
set autoread
set laststatus=2
set backspace=indent,eol,start
set smartindent
set mouse=n
set history=64
set nobackup
set showmatch
set hidden

set nowrapscan

"set expandtab
set tabstop=4
set softtabstop=4
set shiftwidth=4

filetype plugin on
filetype indent on

syntax enable

set guifont=Courier\ 10

"set autowriteall
set hidden

let mapleader = "," " rebind <Leader> key

nnoremap <F5> :GoBuild<CR>
nnoremap <F6> :GoRun<CR>
nnoremap <silent> <F7> :GoErrCheck<CR>
let g:lt_quickfix_list_toggle_map = '<F8>'
nnoremap <silent> <F9> :TagbarToggle<CR>
nnoremap <silent> <F10> :NERDTreeToggle<CR>
nnoremap <silent> <Leader>gg :Ag<CR>

"------------------------------------------------------------
" Show trailing whitespace
autocmd ColorScheme * highlight ExtraWhitespace ctermbg=red guibg=red
au InsertLeave * match ExtraWhitespace /\s\+$/
nnoremap <silent> <Leader>x :%s/\s\+$//<CR>

"------------------------------------------------------------
"wildmode
set wildignore=*.swp,*.pyc,*.gz,*.bz2,*.so,*.so.*,*.tgz,*.zip,tags,tags.*
set wildignore+=.git,.gitignore,.exe,*.out

"------------------------------------------------------------
" Settings for vim-colorscheme
set background=dark
"colorscheme molokai
"colorscheme solarized
colorscheme desert256
set t_Co=256                    "Set terminal color support 256 color
"if has('gui_running')
"  colorscheme desert
"else
"  colorscheme relaxedgreen
"endif

"--------------------------------------------------------------------------------
"cursorline and cursorcolumn
"set cursorline
"hi CursorLine cterm=NONE ctermbg=NONE ctermfg=NONE


"--------------------------------------------------------------------------------
" OmniCppComplete
let OmniCpp_NamespaceSearch = 1
let OmniCpp_GlobalScopeSearch = 1
let OmniCpp_ShowAccess = 1
let OmniCpp_ShowPrototypeInAbbr = 1 " show function parameters
let OmniCpp_MayCompleteDot = 1 " autocomplete after .
let OmniCpp_MayCompleteArrow = 1 " autocomplete after ->
let OmniCpp_MayCompleteScope = 1 " autocomplete after ::
let OmniCpp_DefaultNamespaces = ["std", "_GLIBCXX_STD"]
" automatically open and close the popup menu / preview window
au CursorMovedI,InsertLeave * if pumvisible() == 0|silent! pclose|endif
set completeopt=menuone,menu,longest,preview

"--------------------------------------------------------------------------------
"tagbar
"nnoremap <silent> <F11> :TagbarToggle<CR>
let g:tagbar_autoclose = 0
let g:tagbar_sort = 0
let g:tagbar_indent = 1
"let g:tagbar_iconchars = ['+', '-']
let g:tagbar_left = 1
let g:tagbar_width = 42
let g:tagbar_compact = 1

"------------------------------------------------------------
"nerdtree
autocmd WinEnter * if (winnr("$") == 1 && exists("b:NERDTree") && b:NERDTree.isTabTree()) | q | endif
let NERDTreeWinSize = 82
let NERDTreeWinPos = "right"
let NERDTreeShowHidden = 1
let NERDTreeAutoCenter = 1
let NERDTreeAutoDeleteBuffer = 1
let NERDTreeDirArrowExpandable = '+'
let NERDTreeDirArrowCollapsible = '-'
let NERDTreeRespectWildIgnore = 1
let NERDTreeQuitOnOpen = 1

"------------------------------------------------------------
"minbufexplorer

"------------------------------------------------------------
"ag.vim
let g:ag_prg = "ag --cpp --cc --vimgrep"
let g:ag_working_path_mode = "r"
let g:ag_highlight = 1
let g:ag_qhandler="copen 20"
let g:ag_mapping_message = 0
let g:ag_apply_qmappings = 1

let g:lt_height = 20

"------------------------------------------------------------
"go.vim
let g:go_highlight_functions = 1
let g:go_highlight_methods = 1
let g:go_highlight_fields = 1
let g:go_highlight_types = 1
let g:go_highlight_operators = 1
let g:go_highlight_build_constraints = 1

"let g:go_fmt_command = "goimports"
let g:go_fmt_fail_silently = 1
let g:go_fmt_autosave = 0
let g:go_play_open_browser = 0
let g:go_get_update = 0

autocmd BufWritePre *.go GoFmt
"autocmd BufWritePre *.go GoErrCheck

"-------------------------------------------------------------
"Flake8
let g:flake8_quickfix_height = 7
highlight link Flake8_Error      Error
highlight link Flake8_Warning    WarningMsg
highlight link Flake8_Complexity WarningMsg
highlight link Flake8_Naming     WarningMsg
highlight link Flake8_PyFlake    WarningMsg
autocmd BufWritePost *.py call Flake8()



