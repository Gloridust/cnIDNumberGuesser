function is_valid_id_number(id_number) {
    if (!/^\d{17}[\dxX]$/.test(id_number)) {
      return false;
    }
    const factors = [7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2];
    const id_remainders = ['1', '0', 'x', '9', '8', '7', '6', '5', '4', '3', '2'];
    const total_sum = id_number
      .slice(0, 17)
      .split('')
      .reduce((sum, digit, index) => sum + parseInt(digit) * factors[index], 0);
    const remainder = total_sum % 11;
    return id_remainders[remainder].toLowerCase() === id_number[17].toLowerCase();
  }
  
  function generate_id_options(id_input) {
    const count_asterisks = id_input.split('').filter((char) => char === '*').length;
    if (count_asterisks === 0) {
      return [];
    }
  
    const city_code = id_input.slice(0, 6);
    const birth_year = id_input.slice(6, 10);
    const birth_month = id_input.slice(10, 12);
    const birth_day = id_input.slice(12, 14);
    const tail_digits = id_input.slice(14, 18);
  
    const valid_cities = citycodes.filter((city) => {
      const city_pattern = city_code.replace(/\*/g, '\\d');
      const regex = new RegExp(city_pattern);
      return regex.test(city);
    });
  
    const birth_year_options = [];
    if (birth_year === '****') {
      const currentYear = new Date().getFullYear();
      for (let year = 1949; year <= currentYear; year++) {
        birth_year_options.push(year);
      }
    } else {
      const birth_year_pattern = birth_year.replace(/\*/g, '\\d');
      const regex = new RegExp(birth_year_pattern);
      for (let year = 1949; year <= new Date().getFullYear(); year++) {
        if (regex.test(year.toString())) {
          birth_year_options.push(year);
        }
      }
    }
  
    const birth_month_options = [];
    if (birth_month === '**') {
      for (let month = 1; month <= 12; month++) {
        birth_month_options.push(month.toString().padStart(2, '0'));
      }
    } else {
      const firstDigit = birth_month[0];
      const secondDigit = birth_month[1];
      if (firstDigit === '*' && secondDigit !== '*') {
        for (let month = 1; month <= 12; month++) {
          if (Math.floor(month / 10) === parseInt(secondDigit)) {
            birth_month_options.push(month.toString().padStart(2, '0'));
          }
        }
      } else if (firstDigit !== '*' && secondDigit === '*') {
        for (let month = 10; month <= 12; month++) {
          if (Math.floor(month / 10) === parseInt(firstDigit)) {
            birth_month_options.push(month.toString().padStart(2, '0'));
          }
        }
      } else {
        birth_month_options.push(birth_month);
      }
    }
  
    const birth_day_options = {};
    birth_month_options.forEach((month_option) => {
      if (birth_day === '**') {
        birth_day_options[month_option] = year_days[month_option];
      } else {
        const firstDigit = birth_day[0];
        const secondDigit = birth_day[1];
        if (firstDigit === '*' && secondDigit !== '*') {
          birth_day_options[month_option] = year_days[month_option].filter((day) => Math.floor(day / 10) === parseInt(secondDigit));
        } else if (firstDigit !== '*' && secondDigit === '*') {
          if (parseInt(firstDigit) === 1) {
            birth_day_options[month_option] = year_days[month_option].filter((day) => Math.floor(day / 10) === 1);
          } else if (parseInt(firstDigit) === 0) {
            birth_day_options[month_option] = year_days[month_option].filter((day) => Math.floor(day / 10) === 0);
          } else if (parseInt(firstDigit) === 2) {
            birth_day_options[month_option] = year_days[month_option].filter((day) => Math.floor(day / 10) === 2);
          }
        } else {
          birth_day_options[month_option] = [birth_day];
        }
      }
    });
  
    const tail_options = [];
    if (tail_digits === '****') {
      for (let tail = 0; tail < 10000; tail++) {
        tail_options.push(tail.toString().padStart(4, '0'));
      }
    } else {
      const asteriskCount = tail_digits.split('').filter((char) => char === '*').length;
      if (asteriskCount === 0) {
        tail_options.push(tail_digits);
      } else {
        for (let i = 0; i < Math.pow(10, asteriskCount); i++) {
          const tail = i.toString().padStart(asteriskCount, '0');
          tail_options.push(tail_digits.replace(/\*/g, () => tail.charAt(0)));
        }
      }
    }
  
    const all_options = [];
    valid_cities.forEach((city_option) => {
      birth_year_options.forEach((year_option) => {
        birth_month_options.forEach((month_option) => {
          birth_day_options[month_option].forEach((day_option) => {
            tail_options.forEach((tail_option) => {
              all_options.push(city_option + year_option + month_option + day_option + tail_option);
            });
          });
        });
      });
    });
  
    return all_options.filter((option) => is_valid_id_number(option));
  }
  
  const id_input = prompt('请输入身份证号码，用星号 (*) 替代未知数字：');
  const id_options = generate_id_options(id_input);
  
  console.log('>>> 生成的身份证号码选项:');
  console.log(id_options);
  