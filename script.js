// Initialize AOS
AOS.init();

// Selects elements in index.html
const display = document.querySelector('#display')
const generateButton = document.querySelector("#generate")
const prepasswordinput = document.querySelector('#prepassword')

function generatePassword() {
    const prepassword = parseInt(prepasswordinput.value)
    
    let numberStr = prepassword.toString();
    let digitArray = numberStr.split('').map(Number);

    digitArray[0] = (digitArray[0] + 4 * 12 - 15)%10;
    digitArray[1] = (digitArray[1] + 1 * 11 - 3)%10;
    digitArray[2] = (digitArray[2] + 13 * 10 - 5)%10;
    digitArray[3] = (digitArray[3] + 30 * 7 - 18)%10;
    digitArray[4] = (digitArray[4] + 13 * 8 - 8)%10;
    digitArray[5] = (digitArray[5] + 2 * 3 - 1)%10;
    
    let password = parseInt(digitArray.join(''));

    // Display randomNumber to user
    display.innerHTML = password
}

// Activates function once button is clicked
generateButton.addEventListener("click", generatePassword)
