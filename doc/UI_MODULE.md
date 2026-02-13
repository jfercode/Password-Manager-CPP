# Módulo UI - Interfaz Gráfica de Usuario

## Descripción General
El módulo `ui/` proporciona toda la interfaz gráfica del Password Manager utilizando Qt Framework. Está dividido en tres componentes principales: LoginDialog (autenticación), NewUserDialog (registro), y MainWindow (ventana principal). Qt es un framework de código abierto para crear aplicaciones gráficas multiplataforma con C++.

---

## Archivos Incluidos

### 1. **LoginDialog.hpp**
Archivo de encabezado que define el diálogo de login.

**Ubicación:** `/include/LoginDialog.hpp`

#### Clase: `LoginDialog : public QDialog`
Diálogo modal para la autenticación de usuarios.

**Herencia Qt:**
```cpp
class LoginDialog : public QDialog
{
    Q_OBJECT  // Macro necesario para señales, slots y meta-objetos
}
```

**Miembros Privados:**

1. **Funciones UI:**
   - `void setupUi()` - Construye la interfaz gráfica del diálogo

2. **Componentes Qt:**
   - `AuthenticationManager *authM` - Gestor de autenticación
   - `QLineEdit *userEdit` - Campo de entrada para usuario
   - `QLineEdit *passEdit` - Campo de entrada para contraseña
   - `QPushButton *loginBttn` - Botón para iniciar sesión
   - `QPushButton *cancelBttn` - Botón para cancelar

**Slots Privados (Métodos ejecutados por señales):**
- `void onLoginClicked()` - Ejecutado cuando se hace clic en login
- `void onCancelClicked()` - Ejecutado cuando se hace clic en cancelar

**Constructor:**
```cpp
explicit LoginDialog(QWidget* parent = nullptr, AuthenticationManager *auth = nullptr);
```
- `parent`: Widget padre (nullptr es ventana sin padre)
- `auth`: Puntero a AuthenticationManager para validar credenciales
- Palabra clave `explicit` previene conversiones implícitas

**Destructor:**
```cpp
~LoginDialog();
```

---

### 2. **LoginDialog.cpp**
Implementación del diálogo de login.

**Ubicación:** `/src/ui/LoginDialog.cpp`

**Constructor:**
```cpp
LoginDialog::LoginDialog(QWidget *parent, AuthenticationManager *auth) : QDialog(parent)
{
    setWindowTitle("Login Password Manager");
    PrintLog(std::cout, YELLOW "Login Dialog" RESET " - Initialazing UI...");    
    setupUi();
    authM = auth;
    connect(loginBttn, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(cancelBttn, &QPushButton::clicked, this, &LoginDialog::onCancelClicked);
}
```

**Proceso:**
1. Llama constructor de QDialog con parent
2. Establece título de ventana
3. Configura UI llamando setupUi()
4. Asigna el gestor de autenticación
5. Conecta señales a slots:
   - Cuando se hace clic en loginBttn, ejecuta onLoginClicked()
   - Cuando se hace clic en cancelBttn, ejecuta onCancelClicked()

**`setupUi()`:**
Construye la interfaz gráfica:

```cpp
void LoginDialog::setupUi()
{
    // Crear widgets
    userEdit = new QLineEdit(this);
    passEdit = new QLineEdit(this);
    loginBttn = new QPushButton("Login", this);
    cancelBttn = new QPushButton("Cancelar", this);
    
    // Configurar widgets
    userEdit->setPlaceholderText("Usuario");
    passEdit->setPlaceholderText("Contraseña");
    passEdit->setEchoMode(QLineEdit::Password);  // Oculta caracteres
    
    // Layout vertical principal
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Usuario", this));
    layout->addWidget(userEdit);
    layout->addWidget(new QLabel("Contraseña", this));
    layout->addWidget(passEdit);
    
    // Layout horizontal para botones
    QHBoxLayout *bttnLayout = new QHBoxLayout();
    bttnLayout->addWidget(loginBttn);
    bttnLayout->addWidget(cancelBttn);
    
    // Añadir layout de botones al principal
    layout->addLayout(bttnLayout);
}
```

**Estructura Visual:**
```
┌─────────────────────────────┐
│  Login Password Manager     │
├─────────────────────────────┤
│ Usuario                     │
│ [___________________]       │
│ Contraseña                  │
│ [___________________]       │
│ [Login]       [Cancelar]    │
└─────────────────────────────┘
```

**`onLoginClicked()`:**
```cpp
void LoginDialog::onLoginClicked()
{
    QString user = userEdit->text();
    QString pass = passEdit->text();
    
    if (authM && authM->authenticateUser(user.toStdString(), pass.toStdString()))
        accept();
    else
        QMessageBox::warning(this, "Error", "Credeniales incorrectas");
}
```

**Proceso:**
1. Obtiene textos de los QLineEdit como QString
2. Verifica que authM existe
3. Convierte QString a std::string
4. Llama authenticateUser()
5. Si es exitoso, `accept()` cierra el diálogo con código `QDialog::Accepted`
6. Si falla, muestra MessageBox de error

**`onCancelClicked()`:**
```cpp
void LoginDialog::onCancelClicked()
{
    reject();  // Retorna QDialog::Rejected
}
```

---

### 3. **NewUserDialog.hpp**
Archivo de encabezado para el diálogo de nuevo usuario.

**Ubicación:** `/include/NewUserDialog.hpp`

#### Clase: `NewUserDialog : public QDialog`

**Miembros Privados:**
- `void setupUi()` - Construye interfaz del diálogo

**Constructor:**
```cpp
explicit NewUserDialog(QWidget *parent = nullptr);
```

**Destructor:**
```cpp
~NewUserDialog();
```

**Nota:** Actualmente es un esqueleto, listo para implementación.

---

### 4. **NewUserDialog.cpp**
Implementación del diálogo de nuevo usuario.

**Ubicación:** `/src/ui/NewUserDialog.cpp`

Actualmente contiene solo la declaración de clase, listo para expansión.

---

### 5. **MainWindow.hpp**
Archivo de encabezado para la ventana principal.

**Ubicación:** `/include/MainWindow.hpp`

#### Clase: `MainWindow : public QMainWindow`
Ventana principal de la aplicación después de autenticación.

**Herencia Qt:**
```cpp
class MainWindow : public QMainWindow
{
    Q_OBJECT
}
```

**Miembros Privados:**

1. **Funciones UI:**
   - `void setupUI()` - Construye interfaz principal

2. **Componentes Qt:**
   - `QPushButton *addBttn` - Botón para añadir contraseña
   - `QPushButton *refreshBttn` - Botón para refrescar lista
   - `QPushButton *deleteBttn` - Botón para eliminar contraseña
   - `QPushButton *logoutBttn` - Botón para cerrar sesión

**Slots Privados:**
- `void onClickAddPssBttn()` - Añade nueva contraseña
- `void onClickDeleteBttn()` - Elimina contraseña seleccionada
- `void onClickLogoutBttn()` - Cierra sesión

- `void onViewPassword(int row)` - Visualiza contraseña
- `void onEditPassword(int row)` - Edita contraseña
- `void onDeletePassword(int row)` - Elimina contraseña

**Constructor:**
```cpp
explicit MainWindow(void);
```

**Destructor:**
```cpp
~MainWindow();
```

---

### 6. **MainWindow.cpp**
Implementación de la ventana principal.

**Ubicación:** `/src/ui/MainWindow.cpp`

**Constructor:**
```cpp
MainWindow::MainWindow(void)
{
    setWindowTitle("Password Manager - Secure Storage");
    PrintLog(std::cout, YELLOW "Main Window" RESET " - Initialazing UI...");
    setupUI();
    
    connect(addBttn, &QPushButton::clicked, this, &MainWindow::onClickAddPssBttn);
    connect(deleteBttn, &QPushButton::clicked, this, &MainWindow::onClickDeleteBttn);
    connect(logoutBttn, &QPushButton::clicked, this, &MainWindow::onClickLogoutBttn);
    
    PrintLog(std::cout, YELLOW "Main Window" RESET " - Showing UI...");
    show();
}
```

**`setupUI()`:**
Construye la interfaz principal con:

1. **Configuración de Ventana:**
   - Tamaño: WIDTH x HEIGHT (definidas como constantes)
   - Tamaño mínimo: WIDTH/2 x HEIGHT/2

2. **Estructura de Layout:**
   - **Central Widget:** Contenedor principal
   - **Main Layout (QVBoxLayout):** Contenedor vertical principal
   - **Header Layout (QHBoxLayout):** Sección superior con título
   - **Table Widget (QTableWidget):** Tabla de contraseñas

3. **Tabla de Contraseñas:**
   - Columnas: Website, Username, Password, Actions
   - Ancho columnas configurado
   - Selección de filas completas
   - Colores alternados para mejor legibilidad

4. **Datos de Ejemplo:**
   - Una fila de ejemplo con Gmail (será reemplazada por datos reales)
   - Campos: Website, Username, Password mascarada

5. **Acciones (Botones):**
   - Botón Ver (👁) - Visualizar contraseña
   - Botón Editar (✏️) - Modificar contraseña
   - Botón Eliminar (🗑️) - Borrar contraseña

---

## Qt Framework (https://www.qt.io/)

Qt es el framework más popular para desarrollar aplicaciones gráficas en C++ con multiplataforma.

### Módulos Qt Utilizados:

#### **QtCore**
Módulo base con funcionalidad fundamental:
- **`QObject`** - Clase base para objetos en Qt con sistema de señales/slots
- **`QString`** - Cadena Unicode optimizada para Qt
- **`QWidget`** - Clase base para todos los elementos visuales
- **`QLayout`** - Gestor de diseño automático de widgets

#### **QtGui**
Componentes gráficos:
- **`QDialog`** - Ventana modal para interacción del usuario
- **`QLineEdit`** - Campo de entrada de texto
- **`QPushButton`** - Botón presionable
- **`QLabel`** - Etiqueta de texto
- **`QMessageBox`** - Cuadro de diálogo modal para mensajes
- **`QFont`** - Propiedades de fuente
- **`QTableWidget`** - Tabla editable y seleccionable

#### **QtWidgets**
Widgets de alto nivel:
- **`QMainWindow`** - Ventana principal con menú, barras de herramientas, etc.
- **`QVBoxLayout`** - Distribuye widgets verticalmente
- **`QHBoxLayout`** - Distribuye widgets horizontalmente

### Clases Qt Clave:

#### **QDialog**
```cpp
class QDialog : public QWidget
{
    ...
    int exec();      // Muestra diálogo modal y espera resultado
    void accept();   // Cierra con código QDialog::Accepted (1)
    void reject();   // Cierra con código QDialog::Rejected (0)
};
```

#### **QLineEdit**
```cpp
class QLineEdit : public QWidget
{
    QString text() const;              // Obtiene texto actual
    void setText(const QString &);     // Establece texto
    void setPlaceholderText(...);     // Texto de relleno
    void setEchoMode(EchoMode);       // Password, Normal, etc.
};

enum EchoMode {
    Normal,     // Muestra todos los caracteres
    NoEcho,     // No muestra nada
    Password,   // Muestra puntos en lugar de caracteres
    PasswordEchoOnEdit
};
```

#### **QPushButton**
```cpp
class QPushButton : public QAbstractButton
{
    signal clicked(bool checked = false);  // Señal cuando se hace clic
};
```

#### **QMessageBox**
```cpp
static void warning(QWidget *parent, const QString &title, const QString &text);
static int question(...);
static void information(...);
```

#### **QTableWidget**
```cpp
class QTableWidget : public QTableView
{
    void setColumnCount(int count);
    void setHorizontalHeaderLabels(const QStringList &labels);
    void insertRow(int row);
    void setItem(int row, int column, QTableWidgetItem *item);
    void setColumnWidth(int column, int width);
    void setSelectionBehavior(SelectionBehavior);
    void setAlternatingRowColors(bool);
};
```

---

## Sistema de Señales y Slots

Qt implementa un sistema de programación basado en eventos usando **Señales** y **Slots**.

### Concepto:
- **Señal:** Un evento emitido cuando algo sucede (ej: botón presionado)
- **Slot:** Una función que se ejecuta cuando se recibe una señal
- **Connect:** Vincula una señal a un slot

### Ejemplo:
```cpp
// Cuando se presiona loginBttn, ejecutar onLoginClicked()
connect(loginBttn, &QPushButton::clicked, 
        this, &LoginDialog::onLoginClicked);
```

**Ventajas:**
- Decoupling: Los objetos no necesitan conocerse directamente
- Type-safe: Verificación en tiempo de compilación
- Flexible: Múltiples slots pueden conectarse a una señal

### Ciclo Completo:
```
Usuario presiona botón
    ↓
QPushButton emite señal clicked()
    ↓
Qt invoca slot onLoginClicked()
    ↓
Slot ejecuta lógica de autenticación
    ↓
accept() o reject() cierra diálogo
```

---

## Macro Q_OBJECT

```cpp
class LoginDialog : public QDialog
{
    Q_OBJECT  // <-- NECESARIO para señales y slots
    
private slots:
    void onLoginClicked();
};
```

**Propósito:**
- Expande el sistema de meta-objeto de Qt
- Permite declarar señales y slots
- Genera código de introspección en tiempo de compilación
- Generado automáticamente por Meta-Object Compiler (moc)

**Sin Q_OBJECT:**
- Los slots no funcionan
- Las señales no se emiten
- Fallos en tiempo de compilación

---

## Flujo de Aplicación

```
main()
    ↓
QApplication app(argc, argv)
    ↓
LoginDialog login
    ├─ setupUi() - Construye interfaz
    ├─ connect() - Vincula señales a slots
    ↓
login.exec() - Muestra diálogo modal
    ↓
    ├─ Usuario ingresa credenciales
    ├─ Presiona "Login"
    ├─ onLoginClicked() ejecuta authenticateUser()
    │   ├─ Si exitoso: accept() retorna QDialog::Accepted
    │   └─ Si falla: muestra error
    │
    └─ Usuario presiona "Cancelar"
        └─ onCancelClicked() ejecuta reject() retorna QDialog::Rejected
    ↓
Si QDialog::Accepted:
    MainWindow window
    └─ Muestra ventana principal
        └─ app.exec() - Inicia event loop
else:
    exit(0) - Termina aplicación
```

---

## Características de Componentes Qt

### QLineEdit
- Edición simple de texto
- Validación de entrada (opcional)
- Echo mode para campos de contraseña
- Placeholder text para orientación del usuario

### QPushButton
- Clickeable con mouse
- Enfoque con teclado
- Estados (presionado, hover, etc.)
- Icono y texto

### QDialog
- Ventana modal (bloquea ventana padre)
- Valores de retorno (Accepted/Rejected)
- exec() espera cerrar y retorna código
- accept()/reject() cierran el diálogo

### QTableWidget
- Filas y columnas edibles
- Selección configurable
- Headers personalizables
- Filas coloreadas alternadamente

---

## Consideraciones de Diseño

### 1. **Separación de Responsabilidades:**
- LoginDialog: Apenas UI, delega autenticación a AuthenticationManager
- MainWindow: Muestra datos, no los procesa

### 2. **Inyección de Dependencias:**
```cpp
LoginDialog(QWidget* parent, AuthenticationManager *auth)
```
Recibe las dependencias, no las crea

### 3. **Const Correctness:**
- Los métodos de Qt pueden no ser const (especificidad de Qt)

### 4. **Memory Management Qt:**
- Paternidad de widgets: Los padres liberan a los hijos
- `new` es seguro porque Qt los rastrea

---

## Limitaciones Conocidas

1. **NewUserDialog:** Aún no implementado completamente
2. **MainWindow:** Tabla de ejemplo, no datos reales
3. **Gestión de Contraseñas:** Botones conectados pero funcionalidad vacía
4. **Búsqueda:** Campo de búsqueda comentado en MainWindow

---

## Extensiones Futuras

1. **Implementar NewUserDialog:** Formulario para registro de nuevo usuario
2. **Implementar gestión de contraseñas:**
   - Añadir nuevas contraseñas
   - Ver contraseñas (con autorización)
   - Editar contraseñas existentes
   - Eliminar contraseñas
3. **Barra de búsqueda:** Filtrar contraseñas por sitio web
4. **Temas:** Soporte para modo oscuro/claro
5. **Seguridad adicional:** Timeout de sesión, bloqueo de pantalla
6. **Icono de aplicación:** Reemplazar placeholder

---

## Patrones Qt Utilizados

### Constructor Explícito:
```cpp
explicit LoginDialog(QWidget* parent = nullptr, ...);
```
Previene conversiones implícitas de QWidget a LoginDialog

### Signal-Slot Connection:
```cpp
connect(loginBttn, &QPushButton::clicked, 
        this, &LoginDialog::onLoginClicked);
```
Type-safe con punteros a función miembro

### Structured Binding (C++17):
En setupUi(), construcción y asignación en una línea

---

## Notas de Compilación

Para compilar con Qt, CMakeLists.txt debe incluir:
```cmake
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
target_link_libraries(PasswordManager Qt6::Core Qt6::Gui Qt6::Widgets)
```

El Meta-Object Compiler (moc) procesa automáticamente archivos con Q_OBJECT.
