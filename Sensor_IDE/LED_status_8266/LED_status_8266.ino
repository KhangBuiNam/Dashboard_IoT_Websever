#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// Thông tin Wi-Fi
const char* ssid = "Meo";
const char* password = "zxcvbnml";

// Thông tin Firebase
#define FIREBASE_HOST "iot-webdemo-default-rtdb.firebaseio.com"  // URL Firebase
#define FIREBASE_AUTH "SDJKsByrq1j8viX9umKdwoESBkzh3HSelgHtE8dz" // Mã xác thực Firebase    
// Đối tượng Firebase
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

void setup() {
  // Khởi động Serial Monitor
  Serial.begin(9600);
  delay(500);

  // Kết nối Wi-Fi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Chờ kết nối
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Cấu hình Firebase
  firebaseConfig.host = FIREBASE_HOST;           
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH; 

  // Khởi động Firebase
  Firebase.begin(&firebaseConfig, &firebaseAuth);  
  Firebase.reconnectWiFi(true);
  Firebase.setString(firebaseData, "/led/ledState", "false"); // Truyền đối tượng firebaseData
  pinMode(D1, OUTPUT); // Khởi tạo pin LED
}

void loop() {
  // Lấy giá trị từ Firebase
  if (Firebase.getString(firebaseData, "/led/ledState")) { // Kiểm tra xem có thành công không
    String ledState = firebaseData.stringData(); // Lấy dữ liệu chuỗi
    // Xử lý trạng thái LED
    if (ledState == "true") {
      Serial.println("LED ON");
      digitalWrite(D1, HIGH);
    } else {
      Serial.println("LED OFF");
      digitalWrite(D1, LOW);
    }
  } else {
    Serial.print("Lỗi khi lấy ledState: ");
    Serial.println(firebaseData.errorReason()); // In ra lý do lỗi
  }

  delay(1000); // Thêm độ trễ để tránh việc truy vấn quá nhanh
}
