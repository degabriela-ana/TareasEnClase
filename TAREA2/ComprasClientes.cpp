#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// Clase Producto
class Producto {
private:
    string nombre;
    double precio;
    int stock;

public:
    Producto(string n, double p, int s) : nombre(n), precio(p), stock(s) {}

    string getNombre() { return nombre; }
    double getPrecio() { return precio; }
    int getStock() { return stock; }

    void reducirStock(int cantidad) {
        if (cantidad <= stock) {
            stock -= cantidad;
        }
    }
};

// Clase Cliente
class Cliente {
private:
    string nombre;
    string correo;

public:
    Cliente(string n, string c) : nombre(n), correo(c) {}
    string getNombre() { return nombre; }
    string getCorreo() { return correo; }
};

// Clase Pedido
class Pedido {
private:
    Cliente cliente;
    vector<pair<Producto, int>> carrito; 

public:
    Pedido(Cliente c) : cliente(c) {}

    void agregarProducto(Producto p, int cantidad) {
        carrito.push_back({p, cantidad});
    }

    double calcularTotal() {
        double total = 0;
        for (auto &item : carrito) {
            total += (item.first.getPrecio() * item.second); 
        }
        return total;
    }

    void mostrarResumen() {
        cout << "\n=======================================" << endl;
        cout << "RESUMEN DE COMPRA (EN CONSOLA)" << endl;
        cout << "Cliente: " << cliente.getNombre() << " (" << cliente.getCorreo() << ")" << endl;
        cout << "Productos comprados:" << endl;
        
        for (auto &item : carrito) {
            cout << "- " << item.first.getNombre() 
                 << " | Cantidad: " << item.second 
                 << " | Subtotal: $" << (item.first.getPrecio() * item.second) << endl;
        }
        cout << "Total a pagar: $" << calcularTotal() << endl;
        cout << "=======================================\n" << endl;
    }

    // Guarda el recibo directamente en el archivo de texto
    void guardarEnArchivo() {
        // ios::app sirve para agregar texto al final del archivo sin borrar lo que ya existe
        ofstream archivo("recibos.txt", ios::app); 
        
        if (archivo.is_open()) {
            archivo << "=======================================" << endl;
            archivo << "RECIBO DE VENTA" << endl;
            archivo << "=======================================" << endl;
            archivo << "Cliente: " << cliente.getNombre() << " (" << cliente.getCorreo() << ")" << endl;
            archivo << "Productos:" << endl;
            
            for (auto &item : carrito) {
                archivo << "- " << item.first.getNombre() 
                        << " | Cantidad: " << item.second 
                        << " | Subtotal: $" << (item.first.getPrecio() * item.second) << endl;
            }
            archivo << "Total Gastado: $" << calcularTotal() << endl;
            archivo << "=======================================\n\n";
            archivo.close();
            cout << "-> El recibo se ha guardado en 'recibos.txt' exitosamente." << endl;
        } else {
            cout << "Error: No se pudo escribir en el archivo de recibos." << endl;
        }
    }
};

int main() {
    vector<Cliente> listaClientes;
    vector<Producto> listaProductos;

    // PARA LEER CLIENTES DESDE EL ARCHIVO
    ifstream archClientes("clientes.txt");
    if (!archClientes) {
        cout << "Error: No se pudo abrir clientes.txt" << endl;
        return 1;
    }
    string nomCliente, correo;
    while (archClientes >> nomCliente >> correo) {
        listaClientes.push_back(Cliente(nomCliente, correo));
    }
    archClientes.close();

    // PARA LEER PRODUCTOS DESDE EL ARCHIVO
    ifstream archProductos("productos.txt");
    if (!archProductos) {
        cout << "Error: No se pudo abrir productos.txt" << endl;
        return 1;
    }
    string nomProd;
    double precio;
    int stock;
    while (archProductos >> nomProd >> precio >> stock) {
        listaProductos.push_back(Producto(nomProd, precio, stock));
    }
    archProductos.close();

    char continuarPrograma;
    
    // Permite atender a múltiples personas
    do {
        // PARA ELEGIR CLIENTE
        cout << "\n--- LISTA DE CLIENTES DISPONIBLES ---" << endl;
        for (int i = 0; i < listaClientes.size(); i++) {
            cout << i + 1 << ". " << listaClientes[i].getNombre() << endl;
        }
        
        int opcionCliente;
        cout << "Seleccione el numero del cliente que va a comprar: ";
        cin >> opcionCliente;
        
        // Para validar que la opción sea correcta
        if (opcionCliente < 1 || opcionCliente > listaClientes.size()) {
            cout << "Cliente no valido. Intente de nuevo." << endl;
            continue; 
        }

        // para crear el pedido para el cliente seleccionado
        Pedido pedidoActual(listaClientes[opcionCliente - 1]);

        //  PARA COMPRAR PRODUCTOS
        int opcionProducto;
        do {
            cout << "\n--- CATALOGO DE PRODUCTOS ---" << endl;
            for (int i = 0; i < listaProductos.size(); i++) {
                cout << i + 1 << ". " << listaProductos[i].getNombre() 
                     << " - $" << listaProductos[i].getPrecio() 
                     << " (Stock: " << listaProductos[i].getStock() << ")" << endl;
            }
            cout << "0. Terminar compra de este cliente" << endl;
            cout << "Seleccione un producto para agregar: ";
            cin >> opcionProducto;

            if (opcionProducto > 0 && opcionProducto <= listaProductos.size()) {
                int indice = opcionProducto - 1;
                int cantComprar;
                
                cout << "Cuantos '" << listaProductos[indice].getNombre() << "' desea comprar?: ";
                cin >> cantComprar;

                if (cantComprar <= listaProductos[indice].getStock()) {
                    pedidoActual.agregarProducto(listaProductos[indice], cantComprar);
                    listaProductos[indice].reducirStock(cantComprar);
                    cout << "Producto agregado correctamente." << endl;
                } else {
                    cout << "Error: No hay suficiente stock disponible." << endl;
                }
            }
        } while (opcionProducto != 0);

        // PARA MOSTRAR RESULTADO Y GUARDAR EN ARCHIVO
        pedidoActual.mostrarResumen();
        pedidoActual.guardarEnArchivo();

        // Preguntar si se quiere registrar otra compra antes de cerrar
        cout << "\n¿Desea registrar la compra de otro cliente? (S/N): ";
        cin >> continuarPrograma;

    } while (continuarPrograma == 'S' || continuarPrograma == 's');

    cout << "\nPrograma terminado. Gracias por su compra." << endl;
    return 0;
}
