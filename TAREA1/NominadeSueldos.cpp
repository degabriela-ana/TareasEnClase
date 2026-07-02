#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <cstdlib>

using namespace std;

class Trabajador {
private:
    string nombre;
    string apellido;
    int diasTrabajados;
    float horasPorDia;
    const float TARIFA_HORA = 10.0; // 50 Soles por hora

public:
    Trabajador() : nombre(""), apellido(""), diasTrabajados(0), horasPorDia(0) {}
    
    Trabajador(string n, string a, int d, float h) 
        : nombre(n), apellido(a), diasTrabajados(d), horasPorDia(h) {}
    
    // Setters
    void setNombre(string n) { nombre = n; }
    void setApellido(string a) { apellido = a; }
    void setDiasTrabajados(int d) { diasTrabajados = d; }
    void setHorasPorDia(float h) { horasPorDia = h; }
    
    // Getters
    string getNombre() const { return nombre; }
    string getApellido() const { return apellido; }
    int getDiasTrabajados() const { return diasTrabajados; }
    float getHorasPorDia() const { return horasPorDia; }
    float getTarifaHora() const { return TARIFA_HORA; }
    
    // Calculamos total de horas del mes
    float calcularHorasTotales() const {
        return diasTrabajados * horasPorDia;
    }
    
    // Calculamos sueldo
    float calcularSueldo() const {
        return calcularHorasTotales() * TARIFA_HORA;
    }
    
    // Para mostrar datos
    void mostrarDatos() const {
        cout << "Nombre: " << nombre << " " << apellido << endl;
        cout << "Dias trabajados: " << diasTrabajados << endl;
        cout << "Horas por dia: " << fixed << setprecision(1) << horasPorDia << endl;
        cout << "Total horas del mes: " << fixed << setprecision(1) 
             << calcularHorasTotales() << endl;
        cout << "Sueldo a pagar: S/. " << fixed << setprecision(2) 
             << calcularSueldo() << endl;
        cout << "-----------------------------------" << endl;
    }
};

class GestorNomina {
private:
    vector<Trabajador> trabajadores;
    const string ARCHIVO_DATOS = "trabajadores.txt";
    const string ARCHIVO_SALIDA = "nomina.txt";
    
public:
    // Cargamos los datos de los trabajadores desde archivo
    void cargarTrabajadores() {
        ifstream archivo(ARCHIVO_DATOS);
        
        if (!archivo.is_open()) {
            cout << "No se encontro archivo de datos. Creando uno nuevo..." << endl;
            return;
        }
        
        string nombre, apellido;
        int dias;
        float horas;
        
        while (archivo >> nombre >> apellido >> dias >> horas) {
            trabajadores.push_back(Trabajador(nombre, apellido, dias, horas));
        }
        
        archivo.close();
        cout << "Se cargaron " << trabajadores.size() << " trabajadores." << endl;
    }
    
    // Guardamos los trabajadores en el archivo
    void guardarTrabajadores() {
        ofstream archivo(ARCHIVO_DATOS);
        
        if (!archivo.is_open()) {
            cout << "Error al guardar los datos." << endl;
            return;
        }
        
        for (const auto& t : trabajadores) {
            archivo << t.getNombre() << " " << t.getApellido() << " " 
                    << t.getDiasTrabajados() << " " << fixed << setprecision(1)
                    << t.getHorasPorDia() << endl;
        }
        
        archivo.close();
        cout << "Datos guardados exitosamente:)" << endl;
    }
    
    // Para agregar nuevo trabajador o nuevos trabajadores
    void agregarTrabajador() {
        string nombre, apellido;
        int dias;
        float horas;
        
        cout << "\n--- Agregar Nuevo Trabajador ---" << endl;
        cout << "Nombre: ";
        cin >> nombre;
        cout << "Apellido: ";
        cin >> apellido;
        cout << "Dias trabajados en el mes: ";
        cin >> dias;
        cout << "Horas por dia: ";
        cin >> horas;
        
        if (dias < 0 || horas < 0) {
            cout << "Error: Los valores no pueden ser negativos:(" << endl;
            return;
        }
        
        trabajadores.push_back(Trabajador(nombre, apellido, dias, horas));
        cout << "Trabajador agregado exitosamente." << endl;
    }
    
    // Para mostrar sueldo de un trabajador en específico
    void mostrarSueldoTrabajador() {
        if (trabajadores.empty()) {
            cout << "No hay trabajadores registrados." << endl;
            return;
        }
        
        cout << "\n--- Consultar Sueldo de Trabajador ---" << endl;
        cout << "Trabajadores registrados:" << endl;
        
        for (size_t i = 0; i < trabajadores.size(); i++) {
            cout << i + 1 << ". " << trabajadores[i].getNombre() << " " 
                 << trabajadores[i].getApellido() << endl;
        }
        
        int opcion;
        cout << "Selecciona el numero del trabajador: ";
        cin >> opcion;
        
        if (opcion < 1 || opcion > (int)trabajadores.size()) {
            cout << "Opcion invalida." << endl;
            return;
        }
        
        cout << "\n";
        trabajadores[opcion - 1].mostrarDatos();
    }
    
    // Generamoss reporte de la nómina
    void generarReporteNomina() {
        ofstream archivo(ARCHIVO_SALIDA);
        
        if (!archivo.is_open()) {
            cout << "Error al generar el reporte." << endl;
            return;
        }
        
        archivo << "========== NOMINA DE TRABAJADORES ==========" << endl;
        archivo << "Tarifa horaria: S/. 10.00 por hora" << endl;
        archivo << "===========================================" << endl << endl;
        
        float sueldoTotal = 0;
        
        for (const auto& t : trabajadores) {
            float sueldo = t.calcularSueldo();
            sueldoTotal += sueldo;
            
            archivo << "Nombre: " << t.getNombre() << " " << t.getApellido() << endl;
            archivo << "Dias trabajados: " << t.getDiasTrabajados() << endl;
            archivo << "Horas por dia: " << fixed << setprecision(1) 
                    << t.getHorasPorDia() << endl;
            archivo << "Total horas del mes: " << fixed << setprecision(1) 
                    << t.calcularHorasTotales() << endl;
            archivo << "Sueldo a pagar: S/. " << fixed << setprecision(2) 
                    << sueldo << endl;
            archivo << "-----------------------------------" << endl;
        }
        
        archivo << endl << "RESUMEN:" << endl;
        archivo << "Total trabajadores: " << trabajadores.size() << endl;
        archivo << "Sueldo total a pagar: S/. " << fixed << setprecision(2) 
                << sueldoTotal << endl;
        archivo << "==========================================" << endl;
        
        archivo.close();
        cout << "Reporte generado en '" << ARCHIVO_SALIDA << "'" << endl;
    }
    
    // Mostrar todos los trabajadores
    void mostrarTodosTrabajadores() {
        if (trabajadores.empty()) {
            cout << "No hay trabajadores registrados." << endl;
            return;
        }
        
        cout << "\n--- Lista de Trabajadores ---" << endl;
        for (const auto& t : trabajadores) {
            t.mostrarDatos();
        }
    }
    
    // Menú principal
    void menu() {
        int opcion;
        
        do {
            cout << "\n========== SISTEMA DE NOMINA ==========" << endl;
            cout << "1. Agregar nuevo trabajador" << endl;
            cout << "2. Ver sueldo de un trabajador" << endl;
            cout << "3. Ver todos los trabajadores" << endl;
            cout << "4. Guardar, generar reporte y salir" << endl;
            cout << "======================================" << endl;
            cout << "Selecciona una opcion: ";
            cin >> opcion;
            
            switch (opcion) {
                case 1:
                    agregarTrabajador();
                    break;
                case 2:
                    mostrarSueldoTrabajador();
                    break;
                case 3:
                    mostrarTodosTrabajadores();
                    break;
                case 4:
                    guardarTrabajadores();
                    generarReporteNomina();
                    #ifdef _WIN32
                        system("start nomina.txt");
                    #elif __APPLE__
                        system("open nomina.txt");
                    #endif
                    cout << "Hasta luego!" << endl;
                    break;
                default:
                    cout << "Opcion invalida. Intenta de nuevo." << endl;
            }
        } while (opcion != 4);
    }
};

int main() {
    GestorNomina gestor;
    gestor.cargarTrabajadores();
    gestor.menu();
    
    return 0;
}
