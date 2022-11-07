import random
import torchvision
from torch.utils import data
import torch
from torchvision import transforms
import torch.nn.functional as F
import matplotlib.pyplot as plt
import torch.nn as nn

train_trans = transforms.Compose([
    torchvision.transforms.RandomHorizontalFlip(),
    transforms.ToTensor(),
    transforms.Normalize(0.4814, 0.4565)
])
test_trans = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize(0.4814, 0.4565)
])

batch_size = 64
mnist_train = torchvision.datasets.FashionMNIST(
    root="D:\MyPython\pytorch01\data", train=True, transform=train_trans, download=True)
mnist_test = torchvision.datasets.FashionMNIST(
    root="D:\MyPython\pytorch01\data", train=False, transform=test_trans, download=True)
# print(mnist_train[0][0].shape) torch.Size([1, 28, 28])
train_data = data.DataLoader(mnist_train, shuffle=True, batch_size=batch_size)
test_data = data.DataLoader(mnist_test, shuffle=False, batch_size=batch_size)


class Net(torch.nn.Module):
    def __init__(self) -> None:
        super(Net, self).__init__()
        self.conv1 = nn.Conv2d(1, 32, 3, 1)
        self.conv2 = nn.Conv2d(32, 64, 3, 1)
        self.conv3 = nn.Conv2d(64, 128, 3, 1)
        self.pooling = nn.MaxPool2d(2)
        self.bn1 = nn.BatchNorm2d(32)
        self.bn2 = nn.BatchNorm2d(64)
        self.bn3 = nn.BatchNorm2d(128)
        self.fc1 = nn.Linear(128, 64)
        nn.init.kaiming_uniform_(self.fc1.weight)
        self.fc2 = nn.Linear(64, 10)
        nn.init.kaiming_uniform_(self.fc2.weight)

    def forward(self, x):
        # print("x.shape0", x.shape)
        # x = self.conv1(x)
        # print("x.shape1.1:", x.shape)
        # x = self.pooling(x)
        # print("x.shape1.2:", x.shape)
        # x = torch.relu(x)
        x = torch.relu(self.pooling(self.conv1(x)))
        x = self.bn1(x)
        # print("x.shape1.3:", x.shape)
        x = torch.relu(self.pooling(self.conv2(x)))
        x = self.bn2(x)
        # print("x.shape2:", x.shape)
        x = torch.relu(self.pooling(self.conv3(x)))
        x = self.bn3(x)
        # print("x.shape3:", x.shape)
        x = torch.flatten(x, 1)
        # print("x.shape-:", x.shape)
        x = self.fc1(x)
        x = self.fc2(x)
        return F.log_softmax(x, dim=1)


lr = 0.001
wd = 1e-7
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

model = Net()
model.to(device)
criterion = torch.nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=lr, weight_decay=wd)
running_loss_list = []
batch_idx_list = []


def train(epoch):
    running_loss = 0.0
    for batch_idx, data in enumerate(train_data, 0):
        inputs, target = data
        inputs, target = inputs.to(device), target.to(device)

        optimizer.zero_grad()
        outputs = model(inputs)
        loss = criterion(outputs, target)
        loss.backward()
        optimizer.step()

        running_loss += loss.item()
        if batch_idx % 50 == 49:
            print('[%d, %5d] loss: %.3f' % (epoch + 1, batch_idx + 1, running_loss / 50))
            running_loss_list.append(running_loss / 50)
            running_loss = 0.0


def test():
    correct = 0
    total = 0
    with torch.no_grad():
        for data in test_data:
            images, labels = data
            images, labels = images.to(device), labels.to(device)
            outputs = model(images)
            _, predicted = torch.max(outputs.data, dim=1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()
    accuracy = correct / total
    print('accuracy on test set: %d %% ' % (100 * accuracy))
    return accuracy


epoch_list = []
acc_list = []

for epoch in range(10):
    model.train()
    train(epoch)
    model.eval()
    acc = test()
    epoch_list.append(epoch)
    acc_list.append(acc)
title = f"bs={batch_size}\nlr={lr}\nwd={wd}"
plt.plot(epoch_list, acc_list)
plt.ylabel('accuracy')
plt.xlabel('epoch')
plt.title(title)
plt.savefig(f"./" + title.replace("\n", "-") + "-" + str(random.randint(0, 100)) + ".png")
plt.show()
