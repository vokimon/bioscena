//////////////////////////////////////////////////////////////////////
// Generic C++ Project Makefile v2.1
// Coded by Vokimon (vokimon@jet.es) and Helly
//////////////////////////////////////////////////////////////////////

What is this?

Generic C++ Project Makefile is a makefile that smartly detects all
the C and C++ files in the current directory and build them 
incrementally as the auto-detected dependencies determine.

Features:

- Easyly customizable by changing variables in the first lines.
- Autodetects dependencies between the files in the current directory
- Buildnumber support

Requirements:
- GNU-Make (available in DJGPP, cygwin, Linux...)
- A C compiler
- A build counter (such KKEP Buildnum)

Use:

- Modify the Makefile project variables (compiler options, linker 
options and executable file name)
- Modify the Makefile system variables if your system needs it
(if you want to use different programs to compile, link, delete...)
- Place the source and the makefile on the same directory
- Run 'make dep' (For the first time and when you feel dependencies
(have changed)
- Run 'make'

Legalities:

I get no responability of any damage implied by the use of this
software as expressed in GNU General Public Licence version 2 
available at Software Foundation, Inc., 675 Mass Ave, Cambridge, 
MA 02139, USA.

It would be gently the user to keep the credits in the source 
code in this package (if modified) and in the files it 
generates.

Keep all modifications to this package on public domain if you feel
it would be usefull to other people.

Contact:

vokimon@jet.es
is04069@salleurl.edu


