import random

# ------------------ Constants ------------------
DAYS = ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"]
SHIFTS = ["morning", "afternoon", "evening"]
MAX_DAYS_PER_EMPLOYEE = 5
MIN_EMPLOYEES_PER_SHIFT = 2


# ------------------ Employee Class ------------------
class Employee:
    def __init__(self, name):
        self.name = name
        self.preferred_shifts = {}  # day -> preferred shift
        self.days_worked = 0


# ------------------ Helper Function ------------------
def get_random_employee(available):
    if not available:
        return None
    emp = random.choice(available)
    emp.days_worked += 1
    return emp.name


# ------------------ Main Program ------------------
def main():
    print("Employee Schedule Manager\n")
    num_employees = int(input("Enter number of employees: "))

    employees = []
    for _ in range(num_employees):
        name = input("\nEnter employee name: ")
        emp = Employee(name)

        print("Enter preferred shift (morning/afternoon/evening) for each day:")
        for day in DAYS:
            shift = input(f"  {day}: ").strip().lower()
            emp.preferred_shifts[day] = shift
        employees.append(emp)

    # schedule[day][shift] = list of employee names
    schedule = {day: {shift: [] for shift in SHIFTS} for day in DAYS}

    # ----------- SCHEDULING LOGIC -----------
    for day in DAYS:
        for emp in employees:
            if emp.days_worked >= MAX_DAYS_PER_EMPLOYEE:
                continue

            pref = emp.preferred_shifts[day]
            # assign to preferred shift if available
            if len(schedule[day][pref]) < 3:
                schedule[day][pref].append(emp.name)
                emp.days_worked += 1
            else:
                # Conflict: try another shift on same day
                assigned = False
                for alt in SHIFTS:
                    if len(schedule[day][alt]) < 3:
                        schedule[day][alt].append(emp.name)
                        emp.days_worked += 1
                        assigned = True
                        break

                # Try next day if still unassigned
                if not assigned:
                    for next_day in DAYS:
                        if next_day == day:
                            continue
                        for alt in SHIFTS:
                            if len(schedule[next_day][alt]) < 3 and emp.days_worked < MAX_DAYS_PER_EMPLOYEE:
                                schedule[next_day][alt].append(emp.name)
                                emp.days_worked += 1
                                assigned = True
                                break
                        if assigned:
                            break

    # ----------- ENSURE MIN 2 EMPLOYEES PER SHIFT -----------
    for day in DAYS:
        for shift in SHIFTS:
            while len(schedule[day][shift]) < MIN_EMPLOYEES_PER_SHIFT:
                available = [
                    e for e in employees
                    if e.days_worked < MAX_DAYS_PER_EMPLOYEE and e.name not in schedule[day][shift]
                ]
                chosen = get_random_employee(available)
                if not chosen:
                    break
                schedule[day][shift].append(chosen)

    # ----------- OUTPUT -----------
    print("\n========== FINAL WEEKLY SCHEDULE ==========")
    for day in DAYS:
        print(f"\n{day}:")
        for shift in SHIFTS:
            emps = " ".join(schedule[day][shift])
            print(f"  {shift.title()}: {emps}")
    print("\nScheduling completed successfully!")


# Run the program
if __name__ == "__main__":
    main()

