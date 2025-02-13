describe 'database' do
  def run_script(commands)
    raw_output = nil
    IO.popen("./xdb.exe", "r+") do |pipe|
      commands.each do |command|
        pipe.puts command
      end

      pipe.close_write

      # Read entire output
      raw_output = pipe.gets(nil)
    end
    raw_output.split("\n")
  end

  it 'inserts and retrieves a field' do
    result = run_script([
      "insert 1 user1 person1@example.com",
      "select",
      ".exit",
    ])
    expect(result).to match_array([
      "[INFO] Executed",
      "db > ",
      "db > [0001] user1 : person1@example.com",
      "db > [INFO] Executed",
    ])
  end

  it 'prints error message when table is full' do
    script = (1..1501).map do |i|
      "insert #{i} user#{i} person#{i}@example.com"
    end
    script << ".exit"
    result = run_script(script)
    expect(result[-2]).to eq('db > [ERROR] Table Full')
  end

  it 'prints error messages if string is too long' do
    long_username = "a"*33
    long_email = "a"*257
    script = [
      "insert 1 #{long_username} #{long_email}",
      "select",
      ".exit",
    ]
    result = run_script(script)
    expect(result).to match_array([
      "db > [ERROR] String is too long",
      "db > [INFO] Executed",
      "db > "
    ])
  end

  it 'prints an error message if id is negative' do
    script = [
      "insert -1 foo bar",
      "select",
      ".exit",
    ]
    result = run_script(script)
    expect(result).to match_array([
      "db > [ERROR] Id must be a positive value",
      "db > [INFO] Executed",
      "db > ",
    ])
  end

  it 'keeps data after closing connection' do
    result1 = run_script([
      "insert 1 user1 person1@example.com",
      ".exit",
    ])
    expect(result1).to match_array([
      "db > [INFO] Executed",
      "db > ",
    ])
    result2 = run_script([
      "select",
      ".exit",
    ])
    expect(result2).to match_array([
      "db > [0001] user1 : person1@example.com",
      "db > [INFO] Executed",
      "db > "
    ])
  end
end