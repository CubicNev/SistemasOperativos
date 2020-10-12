# include <stdio.h>
# include <sys/types.h>
# include <unistd.h>
# include <limits.h> // Librearia para tener 

int main(){
    // Traer el nombre del usuario
    char usr[LOGIN_NAME_MAX];
    getlogin_r(usr, LOGIN_NAME_MAX);
    printf("\n El usuario es: %s\n", usr);  

    // Traer el nombre del equipo
    char eqp[HOST_NAME_MAX];
    gethostname(eqp, HOST_NAME_MAX);
    printf("\n El nombre del equipo es: %s\n", eqp);

    // Traer el nombre del directorio actual 
    char dir[PATH_MAX];
    getcwd(dir, PATH_MAX);
    printf("\n El directorio es: %s\n\n", dir);
}