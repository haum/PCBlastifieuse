// Programme régulant la température de la plastifieuse

const int analogInPin = A0;  // Sonde
const int sortieRelai = A1;  // Activation relai
const int sortieLED = 13;

int sensorValue = 0;        // Valeur analogique lue depuis la sonde

// Intervalle de température dans lequel on souhaite se maintenir
//
// TODO: Utiliser un seul seuil au lieu d’un intervalle vu l’inertie thermique
// de 10 °C mesurée
const int MINTEMP = 189;
const int MAXTEMP = 190;

// État du relai, inutilisé pour l’instant
int etatRelai = 0;

// Temps depuis lequel on effectue les mesures (pour le tracé)
float temps = 0;

float sensorToTemp(int sensorValue) {
  // Conversion de la valeur sur 1024 en température en °C (trouvé via reg. linéaire)
  // Vérification : < 4 °C de différence avec valeur sonde JK (quand > 100 °C)
  //                plus grande différence en basses températures (< 15 °C quand < 50°C) car manque de points dans cette zone dans la régression
  // Précision    : ~1.40 °C pour 1/1024 de changement
  return ((float) sensorValue ) * 1.394769865 - 209.9322514775;
}

void setup() {
  // Utilisation d'une entrée analogique en sortie pour faciliter le cablage
  pinMode(sortieRelai, OUTPUT);

  // Utilisation communication série pour relever la température
  Serial.begin(9600);

  // Mise en chauffe du relai au départ
  digitalWrite(sortieRelai, HIGH);
}

void loop() {
  // Lecture de la valeur de la sonde et conversion en température
  sensorValue = analogRead(analogInPin);
  float temp = sensorToTemp(sensorValue);

  if (temp > MAXTEMP) {
    // Trop chaud, on coupe la résistance
    digitalWrite(sortieRelai, LOW);
    digitalWrite(sortieLED, LOW);
    etatRelai = 0;
  } else if (temp < MINTEMP) {
    // Trop froid, on allume la résistance
    digitalWrite(sortieRelai, HIGH);
    digitalWrite(sortieLED, HIGH);
    etatRelai = 1;
  }

  // Données utiles de mesure à envoyer au PC pour contrôle
  Serial.print("temps = ");
  Serial.print(temps);
  Serial.print("\tsensor = " );
  Serial.print(sensorValue);
  Serial.print("\t temperature = ");
  Serial.print(temp);
  Serial.print("\t relai = ");
  Serial.print(etatRelai);
  Serial.print("\n");

  // Attente pour ne pas spammer le port série et pour ne pas commuter le relai
  // trop rapidement si oscillation
  delay(500);

  // Incrémentation du temps en conséquence de cette pause
  temps += 0.5;
}
