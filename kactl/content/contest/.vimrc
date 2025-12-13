set cin aw ai is ts=4 sw=4 tm=50 nu noeb bg=dark ru cul
sy on   |   im jk <esc>   |   im kj <esc>   |   no ; :
" Select region and then type :Hash to hash your selection.
" Useful for verifying that there aren't mistypes.
ca Hash w !cpp -dD -P \| tr -d '[:space:]' \
 \| md5sum \| cut -c-6

========================

" =========================
" Appearance
" =========================
syntax enable
colorscheme elflord
colorscheme jellybeans
set number

" =========================
" Indentation & Tabs
" =========================
set shiftwidth=2
set tabstop=2
set expandtab
set autoindent
set smartindent
set smarttab
set cindent

" =========================
" Editing Behavior
" =========================
set backspace=indent,eol,start
set autochdir

" =========================
" Key Mappings
" =========================
nnoremap <C-s> :w<CR>
inoremap <C-s> <Esc>:w<CR>a

nnoremap <C-l> :tabnext<CR>
nnoremap <C-h> :tabprev<CR>
nnoremap <C-t> :tabnew<CR>
nnoremap <C-w> :q!<CR>

" =========================
" C++ Compile & Run (Windows)
" Assumes MinGW g++ is in PATH
" =========================
augroup cpp_build
  autocmd!
  autocmd FileType cpp nnoremap <C-b> :!g++ % -std=c++11 -O2 -DONPC -o %:r.exe && %:r.exe<CR>
  autocmd FileType cpp nnoremap <C-y> :!g++ % -std=gnu++17 -DONPC -fsanitize=address -g -Wall -Wextra -o %:r.exe && %:r.exe<CR>
augroup END

" =========================
" Template for New C++ Files
" =========================
augroup cpp_template
  autocmd!
  autocmd BufNewFile *.cpp 0r ~/vimfiles/sf/std.cpp
augroup END
