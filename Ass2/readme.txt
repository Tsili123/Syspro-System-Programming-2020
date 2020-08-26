Άρης Τσιλιφώνης 1115201700170 - 2η εργασια συσπρο

Υλοποιησα τα περισσοτερο ερωτηματα με εξαιρεση το topk ranges.
Στο num patient admission/discharge δεν εχω βαλει read στον aggegrator 
γιατι την τελειωσα τελευταια στιγμη.Δεν γινονται επισης τα free 
στον worker που χειριζεται τις δομες.Χρησιμοποιησα blocking pipes για την 
επικοινωνια των διεργασιων.Για τις απαντησεις queries εχω βαλει τις δομες 
απο την προηγουμενη εργασια.Επισης πρατηρησα οτι το parsing του string των 
χωρων στον worker εχει καποια λαθη παρολου που τα directories διαβαζονται σωστα.
Τα signals τα χειριστηκα αποτελεσματικα με δευτερο tty.Εχω signal handlers και στο 
aggegrator(το εχω σαν main στο makefile),και στον worker.Τα read/write γινονται κυριως 
atomic(λιγο μικροτερο απο το max μεγεθος του pipe).Ειχα καποια προβληματα με την εγγραφη 
του logfile στον signal handler για αυτο εβαλα flag εκει που να οριζουν την διαδικασια στο main.
Τελος προτεινω την εκτελεση των ερωτηματων της διαδικασιας κατα σειρα ,γιατι αλλιως δεν εγγυώμαι οτι
θα τρεχει σωστα.Συγκεκριμενα ,ετρεξα ξεχωριστά τα signals με το query.Πριν απο καθε εντολη query τρεχω την εντολη 
/listCountries γιατι αλλιως εκτυπωνονται διαφοροι junk characters.Γινεται καποιο error handling στην εργασια
απλα δεν παραγει errors το script γιατι δεν μπορουσα να εκτυπωσω το error στο αρχειο.
 Τελος οι εντολες που χρησιμοποιησα ειναι οι εξης:

(κυριως με 2 workers).Καλο ειναι να χρησιμοποιησετε τους φακελους μου για την εκτελεση της εργασιας

./main -w 2 -b 10000 -i input_dir

/diseaseFrequency MERS-COV 1900-9-19 2020-10-20 Estonia
/searchPatientRecord
./sb.sh diseasesFile countriesFile input_dir 10 10
/numPatientAdmissions H1N1 1900-9-19 2020-10-20 Estonia
/numPatientDischarges H1N1 1900-9-19 2020-10-20 Estonia